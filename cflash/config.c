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

#include <stdbool.h>
#include <proto/exec.h>
#include <stdio.h>

#include "main.h"
#include "constants.h"
#include "config.h"

/** configure
 *
 * @brief Parse the command arguments and return the config
 * @param argc Arg count
 * @param argv Argument variables
 * @returns Pointer to a Config struct or NULL on error
*/
struct Config* configure(int argc, char *argv[]) {

  bool error = false;

  struct Config *config;
  config = (struct Config *)AllocMem(sizeof(struct Config),MEMF_CLEAR);
  if (config == NULL) return NULL;

  config->op          = OP_NONE;
  config->kick_source = SOURCE_NONE;
  config->ext_source  = SOURCE_NONE;
  config->skipVerify  = false;
  config->programSlot = 0;
  config->eraseBank   = 0;

  for (int i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(argv[i][1]) {

        case 'c':
          if (config->kick_source == SOURCE_NONE) {
            config->kick_source = SOURCE_ROM;
          } else {
            printf("Only one of -c or -k can be specified.\n");
            error = true;
          }
          break;

        case 'C':
          if (config->ext_source == SOURCE_NONE) {
            config->ext_source = SOURCE_ROM;
          } else {
            printf("Only one of -C or -x can be specified.\n");
            error = true;
          }
          break;

        case 'V':
          config->skipVerify = true;
          break;

        case 'E':
          if (config-> op == OP_NONE) {
            config->op = OP_ERASE_CHIP;
          } else {
            error = true;
            printf("Only one operation can be performed at a time.\n");
          }
          break;

        case 'e':
          if (config->op == OP_NONE) {
            config->op = OP_ERASE_BANK;
            if (i+1 < argc) {
              switch (argv[i+1][0]) {
                case 'k':
                  config->eraseBank = FLASH_BANK_1;
                  break;
                
                case 'x':
                  config->eraseBank = FLASH_BANK_0;
                  break;
                
                default:
                  error = true;
                  printf("Invalid bank selected.\n");
                  break;
              }
              i++;
            } else {
              error = true;
              printf("Must specify a bank to erase.\n");
            }
          } else {
            error = true;
            printf("Only one operation can be performed at a time.\n");
          }
          break;

        case 'k':
        case 'K':
          if (config->kick_source == SOURCE_NONE) {
            if (i+1 < argc) {
              config->kick_source = SOURCE_FILE;
              config->ks_filename = argv[i+1];
              i++;
            }
          } else{
            printf("Only one of -c or -k can be specified.\n");
            error = true;
          }
          break;

        case 'x':
          if (config->ext_source == SOURCE_NONE) {
            if (i+1 < argc) {
              config->ext_source = SOURCE_FILE;
              config->ext_filename = argv[i+1];
              i++;
            }
          } else {
            printf("Only one of -C or -x can be specified.\n");
            error = true;
          }
          break;

        case 'I':
          if (i+1 < argc) {
              config->flash_ide_rom = true;
              config->ide_rom_filename = argv[i+1];
              i++;
          }
          break;
        case 'i':
          if (config-> op == OP_NONE) {
            config->op = OP_IDENTIFY;
          } else {
            error = true;
            printf("Only one operation can be performed at a time.\n");
          }
          break;

        case 's':
          if (i+1 < argc) {
            switch (argv[i+1][0]) {
              case '0':
                config->programSlot = 0;
                break;
              
              case '1':
                config->programSlot = 1;
                break;
              
              default:
                error = true;
                printf("Invalid slot selected.\n");
                break;
            }
  
            i++;
          }

          break;

        case 'v':
          if (config->op == OP_NONE) {
            config->op = OP_VERIFY;
          } else {
            error = true;
            printf("Only one operation can be performed at a time.\n");
          }
          break;
      }
    }
  }

  if (config->op == OP_NONE && config->flash_ide_rom == false) {
    if (config->kick_source == SOURCE_NONE && config->ext_source == SOURCE_NONE) {
      error = true;
    } else {
      config->op = OP_PROGRAM;
    }
  }

  if (config->op == OP_VERIFY && config->kick_source == SOURCE_NONE && config->ext_source == SOURCE_NONE) {
    printf("Must specify filename or -c / -C to compare against Kick / Ext ROM\n");
    error = true;
  }

  if (error) {
    FreeMem(config,sizeof(struct Config));
    return (NULL);
  } else {
    return (config);
  }
}

/** usage
 * @brief Print the usage information
*/
void usage() {
    printf("\nUsage: cflash [-iEvV] [-e k|x] [-c|-k <kickstart rom>] [-C|-x <extended rom>] [-I <ide rom>] -s [0|1]\n\n");
    printf("       -c                  -  Copy ROM to Flash.\n");
    printf("       -C                  -  Copy Extended ROM to Flash.\n");
    printf("       -k <kickstart file> -  Kickstart to Flash or verify.\n");
    printf("       -x <ext rom file>   -  Extended ROM to Flash or verify.\n");
    printf("       -i                  -  Print Flash device id.\n");
    printf("       -I <ide rom>        -  Flash IDE ROM.\n");
    printf("       -e [k|x]            -  Erase [k]ickstart or e[x]t rom bank.\n");
    printf("       -E                  -  Erase chip.\n");
    printf("       -v                  -  Verify bank against file or ROM\n");
    printf("       -V                  -  Skip verification after programming.\n");
    printf("       -s [0|1]            -  Select kickstart slot to work on.\n");
}
