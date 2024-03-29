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
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ROM_256K 0x040000
#define ROM_512K 0x080000
#define ROM_1M   0x100000

#define ODD_BANK 0x080000

#define FLASH_BANK_0  0x000000
#define FLASH_BANK_1  0x080000

#define KICK_BASE 0xF80000
#define EXT_BASE  0xF00000
#endif