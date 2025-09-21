# CIDER Software
This repository contains software utilities for the [CIDER](https://github.com/LIV2/CIDER) expansion board for Amiga computers.

## What is CIDER?

CIDER is an expansion board for the Commodore CDTV that significantly enhances its capabilities:
- **11.37 MB Fast RAM**: Including 1.5 MB Ranger RAM, 8 MB standard Fast RAM, and 1.87 MB Bonus RAM at $A00000
- **IDE Interface**: Built-in IDE controller with autoboot driver for hard drives and other storage
- **Dual Kickstart Flash ROM**: Programmable flash memory for multiple Kickstart and Extended ROMs
- **In-system programming**: All flash components can be updated without removing the board
- **CDTV integration**: Plugs into the CDTV's diagnostic port for seamless integration

## Tools Overview

- **[cflash](https://github.com/LIV2/CIDER-Software/tree/main/cflash#cflash)** - Flash ROM programming tool for Kickstart and Extended ROMs
- **[addram](https://github.com/LIV2/CIDER-Software/tree/main/addram#addram)** - Manual tool to add BonusRAM to your system with configurable options
- **[bootrom](https://github.com/LIV2/CIDER-Software/tree/main/bootrom#cider-bootrom)** - Automatic resident module that adds BonusRAM at boot time

## Quick Start

**New to CIDER?** Choose the right tool for your needs:

- **Want automatic memory activation?** → Use **bootrom** (integrate into Extended ROM for set-and-forget operation)
- **Need control over memory settings?** → Use **addram** (command-line tool with options for testing and configuration)
- **Want to update ROMs?** → Use **cflash** (program new Kickstart/Extended ROMs to flash memory)

Each tool has comprehensive documentation with step-by-step instructions for both beginners and advanced users.

## Downloads
Binaries are provided under [Releases](https://github.com/LIV2/CIDER-Software/releases)  
* CIDER-Software.adf and CIDER-Software.lha contain binaries for the tools in this repository
* IDE firmware can be programmed using the lastest [lide-update adf](https://github.com/LIV2/LIDE.device/releases/latest)

## Third-Party notice
lide.device - reloc.S: reloc.S is adapted from the [A4091](https://github.com/A4091/a4091-software) open-source driver and is Copyright Stefan Reinauer  
lide.device: mounter.c is adapted from the [A4091](https://github.com/A4091/a4091-software) open-source driver and is Copyright 2021-2022 Toni Wilen  
lide.device: The fast read/write routines for ATA devices are adapted from [Frédéric REQUIN](https://github.com/fredrequin)'s [at_apollo_device](https://github.com/fredrequin/at_apollo_device)  

## License
All software contained that is not provided by a third-party is covered by a GPL 2.0 Only license  
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)


CIDER and it's software are licensed under the GPL-2.0 only license