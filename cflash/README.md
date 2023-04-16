# CFLASH

A tool to program the Kickstart and IDE Flash of the CIDER board.

## Usage
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