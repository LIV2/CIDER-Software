# CIDER Software
This repository contains software utilities for the [CIDER](https://github.com/LIV2/CIDER) CDTV Expansion

## Contents
- [addram](https://github.com/LIV2/CIDER-Software/tree/main/addram) Adds the A0/Bonus RAM to the system
- [bootrom](https://github.com/LIV2/CIDER-Software/tree/main/bootrom) A Kickstart module to add the Bonus RAM to the system at boot
- [cflash](https://github.com/LIV2/CIDER-Software/tree/main/cflash) A tool to manage the Kick Flash and update the IDE ROM
- [iderom](https://github.com/LIV2/CIDER-Software/tree/main/iderom) The IDE device driver ROM

## Downloading
Binaries are provided under [Releases](https://github.com/LIV2/CIDER-Software/releases)  
* CIDER-IDE-Update.adf is a bootable floppy image that will update the IDE to the latest firmware version
* CIDER-Software.adf and CIDER-Software.lha contain binaries for the tools in this repositiory

## Third-Party notice
iderom - reloc.S: reloc.S is adapted from the [A4091](https://github.com/A4091/a4091-software) open-source driver and is Copyright Stefan Reinauer  
liv2ride.device: mounter.c is adapted from the [A4091](https://github.com/A4091/a4091-software) open-source driver and is Copyright 2021-2022 Toni Wilen  

## License
All software contained that is not provided by a third-party is covered by a GPL 2.0 Only license  
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)


CIDER and it's software are licensed under the GPL-2.0 only license