# CFLASH

A tool to program the Kickstart and Extended ROM flash of the CIDER board for the Commodore CDTV.
This tool allows you to program the flash in-system even while it is currently in use.

## What is CFLASH?

CFLASH is a tool that lets you update the firmware (ROM files) on your CIDER expansion board for the Commodore CDTV. It can update two types of firmware:

- **Kickstart ROM**: The main system firmware that boots your Amiga
- **Extended ROM**: Additional firmware for CDTV functionality

## Before You Start

### Getting Started
- CFLASH only programs the flash memory - it never touches your original system ROMs
- Make sure your CDTV has stable power during flashing
- **If you want to go back**: Simply switch off the kickflash DIP switch and reboot

### What You'll Need
- Your CIDER-equipped CDTV
- ROM files you want to flash (`.rom` files)
- The cflash program (available in [CIDER-Software.adf](https://github.com/LIV2/CIDER-Software/releases/latest))
- A CLI/terminal or Workbench to run commands

### Prerequisites
- CIDER expansion board must be properly installed
- ROM overlay should be switched off
- Bonus RAM must be disabled during flashing

## Common Tasks

### 1. Copy Your Current ROMs to Flash

To preserve what you currently have running:

```
cflash -c -C
```

This copies your current Kickstart and Extended ROMs from the system to flash.

### 2. Flash a New Kickstart ROM

To install a new Kickstart ROM file:

```
cflash -k kick31.rom
```

Replace `kick31.rom` with the actual filename of your Kickstart ROM.

**For dual Kickstart setups:**
- Flash to slot 0 (default): `cflash -s 0 -k kick31.rom`
- Flash to slot 1: `cflash -s 1 -k kick13.rom`

### 3. Flash an Extended ROM

To install a new Extended ROM file:

```
cflash -x cdtv-ext.rom
```

Replace `cdtv-ext.rom` with your Extended ROM filename.

### 4. Copy Current System ROMs to Flash

If you want to preserve your currently running ROMs to flash:

**Copy current Kickstart:**
```
cflash -c
```

**Copy current Extended ROM:**
```
cflash -C
```

**Copy both:**
```
cflash -c -C
```

### 5. Verify Flash Contents

To check if your flash matches a ROM file:

```
cflash -v -k kick31.rom
```

To verify against currently running ROMs:
```
cflash -v -c -C
```

### 6. Check Flash Device Information

To see what flash chip you have:

```
cflash -i
```

This displays the manufacturer and device ID of your flash chip.

## Advanced Operations

### Erasing Flash

**Erase Kickstart bank:**
```
cflash -e k
```

**Erase Extended ROM bank:**
```
cflash -e x
```

**Erase entire chip:**
```
cflash -E
```

### Skip Verification

```
cflash -V -k kick31.rom
```

The `-V` flag skips verification after programming (not recommended).

## Understanding Slots

The CIDER has dual Kickstart slots, allowing you to have two different Kickstart ROMs:

- **Slot 0**: Default slot
- **Slot 1**: Alternative slot

You can switch between them using CIDER's controls. Use `-s 0` or `-s 1` to specify which slot to program.

## ROM File Sizes

CFLASH supports these ROM sizes:

- **Kickstart**: 256KB, 512KB, or 1MB
- **Extended ROM**: 256KB or 512KB

## Troubleshooting

### "Flash is enabled" Message
If you see this message, cflash will automatically:
1. Copy ROMs to RAM
2. Switch to RAM mode
3. Proceed with flashing

This is normal - just wait for it to complete.

### "Bonus RAM must be disabled"
- Restart your system
- Make sure Bonus RAM is not activated before running cflash

### "Unknown Flash device"
- Check that your CIDER board is properly installed
- Verify you're using compatible flash chips

### Verification Failed
- Try flashing again
- Check that your ROM file isn't corrupted
- Ensure stable power during flashing

### Want to Use Original ROMs Again
- Switch off the kickflash DIP switch on your CIDER board
- Reboot your system (it will use the original ROMs)
- Your original ROMs are always safe and untouched

## Example Workflows

### Upgrading to Kickstart 3.1
```
# 1. Copy current setup to flash (optional)
cflash -c -C

# 2. Flash new Kickstart
cflash -k kick31.rom

# 3. Verify it worked
cflash -v -k kick31.rom
```

### Setting up Dual Kickstart
```
# Flash Kickstart 3.1 to slot 0
cflash -s 0 -k kick31.rom

# Flash Kickstart 1.3 to slot 1
cflash -s 1 -k kick13.rom

# Now you can switch between them using CIDER controls
```

### Complete ROM Update
```
# Update both ROM components
cflash -k kick31.rom
cflash -x cdtv-ext.rom
```

## Getting Help

If cflash won't run or you get errors, try running it without parameters to see the usage information:

```
cflash
```

This will show you all available options and their syntax.

## Command-line options
```
Usage: cflash [-iEvV] [-e k|x] [-c|-k <kickstart rom>] [-C|-x <extended rom>] -s [0|1]
       -c                  -  Copy ROM to Flash.
       -C                  -  Copy Extended ROM to Flash.
       -k <kickstart file> -  Kickstart to Flash or verify.
       -x <ext rom file>   -  Extended ROM to Flash or verify.
       -i                  -  Print Flash device id.
       -e [k|x]            -  Erase [k]ickstart or e[x]t rom bank.
       -E                  -  Erase chip.
       -v                  -  Verify bank against file or ROM
       -V                  -  Skip verification after programming.
       -s [0|1]            -  Select kickstart slot to work on.
```

## Final Tips

1. **Start simple**: Begin with copying your current ROMs to flash
2. **Keep ROM files**: Save copies of useful ROM files for future use
3. **Easy recovery**: If something doesn't work, just switch off the kickflash DIP switch and reboot

## License
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)


cflash is licensed under the GPL-2.0 only license