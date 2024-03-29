// SPDX-License-Identifier: GPL-2.0-only
/* This file is part of cflash
 * Copyright (C) 2023 Matthew Harlum <matt@harlum.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <exec/execbase.h>
#include <proto/exec.h>
#include <proto/expansion.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <proto/dos.h>
#include <dos/dos.h>

#include "kick_flash.h"
#include "ide_flash.h"
#include "main.h"
#include "config.h"
#include "cider.h"
#include "constants.h"

#define MANUF_ID       5194
#define PROD_ID        6
#define IDE_MANUF_ID   5194
#define IDE_PROD_ID    5
#define IDE_ROM_OFFSET 0x0

#define CFLASH_VER 1

struct Library *DosBase;
struct ExecBase *SysBase;
struct ExpansionBase *ExpansionBase = NULL;

volatile UWORD *controlRegister;

extern void *flashbase;

int main(int argc, char *argv[])
{
  SysBase = *((struct ExecBase **)4UL);
  DosBase = OpenLibrary("dos.library",0);
  ULONG hw_ver;

  int rc = 0;

  if (DosBase == NULL) {
    return(rc);
  }

  printf("CIDER FlashROM tool\n");

  struct Config *config;
  struct Task *task = FindTask(0);
  SetTaskPri(task,20);
  if ((config = configure(argc,argv)) != NULL) {
    if ((ExpansionBase = (struct ExpansionBase *)OpenLibrary("expansion.library",0)) != NULL) {

      struct ConfigDev *cd = NULL;

      if ((cd = (struct ConfigDev*)FindConfigDev(NULL,MANUF_ID,PROD_ID))) {

        hw_ver = cd->cd_Rom.er_SerialNumber;

        if (hw_ver > CFLASH_VER) {
          printf("A newer version of cflash is needed for this CIDER firmware version.\n");
          rc = 5;
          goto exit;
        } else if (hw_ver < CFLASH_VER) {
          printf("The CIDER firmware must be upgraded to be compatible with this version of cflash\n");
          rc = 5;
          goto exit;
        }

        controlRegister = cd->cd_BoardAddr;


        if (config->op != OP_NONE) {
          // Check that Kick Flash is disabled

          if (*controlRegister & FLASHEN_BIT) {
            printf("Flash is enabled, copying ROM to RAM and disabling Flash\n");
            CopyMem((ULONG *)0xF00000,(ULONG *)0xF00000,1024*1024); // Copy Extended and Kick rom to RAM
            printf("Switching to RAM now, good luck!\n");
            *controlRegister = (CTRL_SET | MAPRAMEN_BIT);
          }

          selectSlot(config->programSlot); // Set the high/low bank select bit for the flash
          // Check that BonusRAM has not been activated yet by AddRam or the bootrom
          if (!(*controlRegister & BONUSEN_BIT)) {

            UWORD manufacturerId, deviceId;

            bool check_device = kick_flash_init(&manufacturerId,&deviceId);

            if (check_device == false && config->op != OP_IDENTIFY) {

              printf("Error: Unknown Flash device Manufacturer: %04X Device: %04X\n", manufacturerId, deviceId);
              printf("Check that ROM overlay is switched off and try again.\n");
              rc = 5;

            } else {

              switch (config->op) {

                case OP_IDENTIFY:
                  printf("Manufacturer: %04X, Device: %04X\n",manufacturerId, deviceId);
                break;

                case OP_VERIFY:
                  // Verify Kickstart
                  if (config->kick_source == SOURCE_ROM) {
                    rc = (verifyBank((ULONG *)KICK_BASE,FLASH_BANK_1,ROM_512K)) ? 0 : 5;
                  } else if (config->kick_source == SOURCE_FILE) {
                    rc = (verifyFile(config->ks_filename,FLASH_BANK_1)) ? 0 : 5;
                  }

                  // Verify Extended ROM
                  if (config->ext_source == SOURCE_ROM) {
                    rc = (verifyBank((ULONG *)EXT_BASE,FLASH_BANK_0,ROM_256K)) ? 0 : 5;
                  } else if (config->ext_source == SOURCE_FILE) {
                    rc = (verifyFile(config->ext_filename,FLASH_BANK_0)) ? 0 : 5;
                  }
                  break;

                case OP_ERASE_BANK:
                  erase_bank(config->eraseBank,config->programSlot);
                  break;

                case OP_ERASE_CHIP:
                  erase_chip();
                  break;

                case OP_PROGRAM:
                  // Kickstart flash
                  if (config->kick_source == SOURCE_ROM) {
                    erase_bank(FLASH_BANK_1,config->programSlot);
                    printf("Copying Kickstart ROM\n");
                    if (copyBufToFlash((void *)KICK_BASE,FLASH_BANK_1,ROM_512K,config->skipVerify) == false) {
                      rc = 5;
                      goto exit;
                    }
                  } else if (config->kick_source == SOURCE_FILE) {
                    ULONG romSize = 0;
                    printf("Flashing kick file %s\n",config->ks_filename);
                    if ((romSize = getFileSize(config->ks_filename)) != 0) {
                      if (romSize == ROM_256K || romSize == ROM_512K ) {
                        erase_bank(FLASH_BANK_1,config->programSlot);
                        if (copyFileToFlash(config->ks_filename,FLASH_BANK_1,romSize,config->skipVerify) == false) {
                          rc = 5;
                          goto exit;
                        }
                      } else if (romSize == ROM_1M) {
                        erase_bank(FLASH_BANK_0,config->programSlot);
                        erase_bank(FLASH_BANK_1,config->programSlot);
                        if (copyFileToFlash(config->ks_filename,FLASH_BANK_0,romSize,config->skipVerify) == false) {
                          rc = 5;
                          goto exit;
                        }
                      } else {
                        printf("Bad file size, 256K/512K/1M ROM required.\n");
                        rc = 5;
                      }
                    }
                  }
                  // Ext ROM flash
                  if (config->ext_source == SOURCE_ROM) {
                    if (*((UWORD *)EXT_BASE + 1) != 0x4EF9) {
                      printf("Extended ROM not found - Check that JP15 is connected and that both A500 mode and Kick Flash are disabled.\n");
                      rc = 5;
                      goto exit;
                    }
                    erase_bank(FLASH_BANK_0,config->programSlot);
                    printf("Copying Extended ROM\n");
                    if (copyBufToFlash((ULONG *)EXT_BASE,FLASH_BANK_0,ROM_256K,config->skipVerify) == false) {
                      rc = 5;
                      goto exit;
                    }
                  } else if (config->ext_source == SOURCE_FILE) {
                    ULONG romSize = 0;
                    printf("Flashing ext file %s\n",config->ext_filename);
                    if ((romSize = getFileSize(config->ext_filename)) != 0) {
                      if (romSize == ROM_256K || romSize == ROM_512K ) {
                        erase_bank(FLASH_BANK_0,config->programSlot);
                        if (copyFileToFlash(config->ext_filename,FLASH_BANK_0,romSize,config->skipVerify) == false) {
                          rc = 5;
                          goto exit;
                        }
                      } else {
                        printf("Bad file size, 256K/512K EXT ROM required.\n");
                        rc = 5;
                      }
                    }
                  }

                  break;

                  case OP_NONE:
                    if (config->flash_ide_rom == false) usage();
                    break;
              }
            }
          } else {
            printf("Bonus RAM must be disabled to program Flash.\n");
            rc = 5;
          }

        }

      } else {
        printf("Couldn't find board with Manufacturer/Prod ID of %d:%d\n",MANUF_ID,PROD_ID);
        rc = 5;
      }

      if (config->flash_ide_rom == true) {
        void *ide_flashbase = NULL;
        struct ConfigDev *ide_configDev;
        if ((ide_configDev = FindConfigDev(NULL,IDE_MANUF_ID,IDE_PROD_ID))) {

          ide_flashbase = ide_configDev->cd_BoardAddr;

          if (ide_configDev->cd_BoardSize > 65536) {
            // Newer version of the CIDER firmware give the IDE device a 128K block, with the top 64K dedicated to the Flash
            // This means we can flash the IDE ROM without having to disable IDE
            ide_flashbase += 65536;
          }

          UBYTE manufId,devId;
          if (ide_flash_init(&manufId,&devId,(ULONG *)ide_flashbase)) {

            ULONG romSize = getFileSize(config->ide_rom_filename);
            if (romSize <= (0x10000 - IDE_ROM_OFFSET)/2) {

              APTR buffer = readFileToBuf(config->ide_rom_filename);
              if (buffer) {

                UBYTE *sourcePtr = NULL;
                UBYTE *destPtr = ide_configDev->cd_BoardAddr;

                printf("Erasing IDE Flash...\n");
                ide_flash_erase_chip();
                
                int progress = 0;
                int lastProgress = 1;

                fprintf(stdout,"Writing IDE ROM:     ");
                fflush(stdout);

                for (int i=0; i<romSize; i++) {

                  progress = (i*100)/(romSize-1);

                  if (lastProgress != progress) {
                    fprintf(stdout,"\b\b\b\b%3d%%",progress);
                    fflush(stdout);
                    lastProgress = progress;
                  }
                  sourcePtr = ((void *)buffer + i);
                  ide_flash_writeByte(i+IDE_ROM_OFFSET,*sourcePtr);

                }

                fprintf(stdout,"\n");
                fflush(stdout);

                fprintf(stdout,"Verifying IDE ROM:     ");
                for (int i=0; i<romSize; i++) {

                  progress = (i*100)/(romSize-1);

                  if (lastProgress != progress) {
                    fprintf(stdout,"\b\b\b\b%3d%%",progress);
                    fflush(stdout);
                    lastProgress = progress;
                  }
                  sourcePtr = ((void *)buffer + i);
                  destPtr = ((void *)ide_flashbase + (i << 1));
                  if (*sourcePtr != *destPtr) {
                        printf("\nVerification failed at %06x - Expected %02X but read %02X\n",(int)destPtr,*sourcePtr,*destPtr);
                        rc = 5;
                        goto exit;
                      }

                }

                fprintf(stdout,"\n");
                fflush(stdout);

                if (buffer) FreeMem(buffer,romSize);
              }

            } else {
              printf("File too large to fit IDE ROM\n");
              rc = 5;
            }

          } else {
            printf("Error: IDE - Unknown Flash device Manufacturer: %02X Device: %02X\n", manufId, devId);
            if (ide_configDev->cd_BoardSize == 65535) {
              printf("Turn IDE off and try again.\n");
            }
            rc = 5;
          }          
        } else {
          printf("Could not find IDE board.\n");
          rc = 5;
        }
      }
    } else {
      printf("Couldn't open Expansion.library.\n");
      rc = 5;
    }

  } else {
    usage();
  }

exit:

  if (config)        FreeMem(config,sizeof(struct Config));
  if (ExpansionBase) CloseLibrary((struct Library *)ExpansionBase);
  if (DosBase)       CloseLibrary((struct Library *)DosBase);

  return (rc);
}

/**
 * erase_bank
 *
 * @brief Erase a bank
 * @param bank Address of the bank to erase
 * @param slot Slot the bank is in that you wish to erase
*/
void erase_bank(ULONG bank,UBYTE slot) {
  bank >>= 19;

  printf("Erasing slot %d bank %d\n", (int)slot, (int)bank);
  bank |= (slot << 1);
  selectSlot(slot);
  kick_flash_erase_bank(bank);
}

/**
 * erase_chip
 *
 * @brief Completely erase the flash
*/
void erase_chip() {
  printf("Erasing chip...");
  kick_flash_erase_chip();
  printf(" Done\n");
}

/**
 * getFileSize
 *
 * @brief return the size of a file in bytes
 * @param filename file to check the size of
 * @returns File size in bytes
*/
ULONG getFileSize(char *filename) {
  BPTR fileLock;
  ULONG fileSize = 0;
  struct FileInfoBlock *FIB;

  FIB = (struct FileInfoBlock *)AllocMem(sizeof(struct FileInfoBlock),MEMF_CLEAR);

  if ((fileLock = Lock(filename,ACCESS_READ)) != 0) {

    if (Examine(fileLock,FIB)) {
      fileSize = FIB->fib_Size;
    }

  } else {
    printf("Error opening %s\n",filename);
  }

  if (fileLock) UnLock(fileLock);
  if (FIB) FreeMem(FIB,sizeof(struct FileInfoBlock));

  return (fileSize);
}

/**
 * readFileToBuF
 *
 * @brief Read the rom file to a buffer
 * @param filename Name of the file to open
 * @return Pointer to a buffer
*/
APTR readFileToBuf(char *filename) {
  ULONG romSize = getFileSize(filename);
  if (romSize == 0) return NULL;

  BPTR fh;
  APTR buffer;

  buffer = AllocMem(romSize,MEMF_ANY);
  if (buffer) {
    fh = Open(filename,MODE_OLDFILE);

    if (fh) {
      Read(fh,buffer,romSize);
      Close(fh);
    } else {
      printf("Error opening %s\n",filename);
      return NULL;
    }

  } else {
    printf("Couldn't allocate memory.\n");
    return NULL;
  }

  return buffer;
}

/**
 * copyFileToFlash
 *
 * @brief Copy the contents of the specified file to the flash
 * @param filename A pointer to the source buffer
 * @param destination Bank address to write to
 * @param romSize Size in bytes of the source
 * @param skipVerify Skip verification
*/
bool copyFileToFlash(char *filename, ULONG destination, ULONG romSize, bool skipVerify) {
  APTR buffer;
  bool success = true;

  if ((buffer = readFileToBuf(filename)) != NULL) {
    success = copyBufToFlash(buffer,destination,romSize,skipVerify);
    FreeMem(buffer,romSize);
  }

  return success;
}

/**
 * copyBufToFlash
 *
 * @brief Copy the contents of the buffer to the flash
 * @param source A pointer to the source buffer
 * @param destination Bank address to write to
 * @param romSize Size in bytes of the source
 * @param skipVerify Skip verification
*/
bool copyBufToFlash(ULONG *source, ULONG destination, ULONG romSize, bool skipVerify) {
  bool success = true;
  int progress = 0;
  int lastProgress = 1;

  UWORD *sourcePtr = NULL;

  ULONG byteCount = (romSize == ROM_256K) ? ROM_512K : romSize; // For 256K ROMs fill up a 512K bank

  fprintf(stdout,"Writing:     ");
  fflush(stdout);
  kick_flash_unlock_bypass(); // Enter unlock bypass mode

  for (ULONG i=0; i<byteCount; i+=2) {
    sourcePtr = ((void *)source + (i % romSize)); // Loop the source address around when programming 256K
    progress = i*100/(byteCount-2);

    if (lastProgress != progress) {
        fprintf(stdout,"\b\b\b\b%3d%%",progress);
        fflush(stdout);
        lastProgress = progress;
    }

    kick_flash_bypass_program(destination+i,*sourcePtr);

  }

  kick_flash_unlock_bypass_reset(); // Return to read mode
  printf("\n");
  if (skipVerify == false) {
    success = verifyBank(source,destination,romSize);
  }

  return success;
}

/** verifyBank
 *
 * @brief compare the specified bank with a buffer
 * @returns success
 * @param source A pointer to the source buffer
 * @param bank The bank address to compare
*/
bool verifyBank(ULONG *source, ULONG bank, ULONG romSize) {
  fprintf(stdout,"Verifying:     ");
  fflush(stdout);

  ULONG progress     = 0;
  ULONG lastProgress = 1;

  UWORD flash_data    = 0;
  ULONG flash_address = 0;
  UWORD source_data   = 0;

  ULONG byteCount = (romSize == ROM_256K) ? ROM_512K : romSize; // For 256K ROMs fill up a 512K bank

  for (ULONG i=0; i<byteCount; i+=2) {

    progress = i*100/(byteCount-2);

    if (lastProgress != progress) {
        fprintf(stdout,"\b\b\b\b%3d%%",(int)progress);
        fflush(stdout);
        lastProgress = progress;
    }

    flash_address = (ULONG)flashbase + bank + i;
    flash_data    = *(UWORD *)(void *)flash_address;
    source_data   = *(UWORD *)((void *)source + (i % romSize)); // Loop the source address around when programming 256K

    if (flash_data != source_data) {
      printf("\nVerification failed at %06x - Expected %04X but read %04X\n",(int)flash_address,source_data,flash_data);
      return false;
    }

  }
  printf("\n");
  return true;
}

/**
 * verifyFile
 *
 * @brief Compare the specified bank with a file
 * @returns success
 * @param filename Filename
 * @param bank Bank address to compare
*/
bool verifyFile(char *filename, ULONG bank) {
  ULONG romSize;
  APTR buffer;

  bool success = false;

  if ((romSize = getFileSize(filename)) != 0) {
    if (romSize == ROM_256K || romSize == ROM_512K || romSize == ROM_1M) {
        if ((buffer = readFileToBuf(filename)) != NULL) {
          if (romSize == ROM_1M) bank &= ~(ODD_BANK); // Force alignment to Bank 0 or 2 for 1MB ROM
          success = verifyBank(buffer,bank,romSize);
          FreeMem(buffer,romSize);
      }
    } else {
      printf("Bad file size, 256K/512K/1M ROM required.\n");
      return false;
    }
  } else {
    return false;
  }
  return success;
}

/**
 * selectSlot
 *
 * @brief Set the high/low 1MB bank of the Flash chip
 * @param slot Slot to select
*/
void selectSlot(UBYTE slot) {
  if (slot == 1) {
    *controlRegister = SET_BANKBIT;
  } else {
    *controlRegister = CLR_BANKBIT;
  }
}
