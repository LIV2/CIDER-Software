# CFLASH

A tool to program the Kickstart and IDE Flash of the CIDER board.
This tool allows you to program the Kick and IDE flash in-system even while it is currently in use

## Usage examples

### Copying current kickstart and extended rom to flash
```cflash -C -c```

### Flashing IDE ROM
```cflash -I cider-ide.rom```

### Flashing a kickstart file to the second slot
```cflash -s 1 -k kick13.rom```

## Command-line options
```
Usage: cflash [-iEvV] [-e k|x] [-c|-k <kickstart rom>] [-C|-x <extended rom>] [-I <ide rom>] -s [0|1]
       -c                  -  Copy ROM to Flash.
       -C                  -  Copy Extended ROM to Flash.
       -k <kickstart file> -  Kickstart to Flash or verify.
       -x <ext rom file>   -  Extended ROM to Flash or verify.
       -i                  -  Print Flash device id.
       -I <ide rom>        -  Flash IDE ROM.
       -e [k|x]            -  Erase [k]ickstart or e[x]t rom bank.
       -E                  -  Erase chip.
       -v                  -  Verify bank against file or ROM
       -V                  -  Skip verification after programming.
       -s [0|1]            -  Select kickstart slot to work on.
```

## License
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)


cflash is licensed under the GPL-2.0 only license