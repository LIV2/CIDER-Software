# CIDER Software
This repository contains software utilities for the [CIDER](https://github.com/LIV2/CIDER) CDTV Expansion

## Contents
- [addram](https://github.com/LIV2/CIDER-Software/tree/main/addram) Adds the A0/Bonus RAM to the system
- [bootrom](https://github.com/LIV2/CIDER-Software/tree/main/bootrom) A Kickstart module to add the Bonus RAM to the system at boot
- [cflash](https://github.com/LIV2/CIDER-Software/tree/main/cflash) A tool to manage the Kick Flash and update the IDE ROM

## Downloads
Binaries are provided under [Releases](https://github.com/LIV2/CIDER-Software/releases)  
* CIDER-Software.adf and CIDER-Software.lha contain binaries for the tools in this repositiory
* IDE firmware can be programmed using the lastest [lide-update adf](https://github.com/LIV2/LIDE.device/releases/latest)

## Third-Party notice
lide.device - reloc.S: reloc.S is adapted from the [A4091](https://github.com/A4091/a4091-software) open-source driver and is Copyright Stefan Reinauer  
lide.device: mounter.c is adapted from the [A4091](https://github.com/A4091/a4091-software) open-source driver and is Copyright 2021-2022 Toni Wilen  
lide.device: The fast read/write routines for ATA devices are adapted from [Frédéric REQUIN](https://github.com/fredrequin)'s [at_apollo_device](https://github.com/fredrequin/at_apollo_device)  

## License
All software contained that is not provided by a third-party is covered by a GPL 2.0 Only license  
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)


CIDER and it's software are licensed under the GPL-2.0 only license