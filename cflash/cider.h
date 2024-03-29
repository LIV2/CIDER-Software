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
#define SET_BANKBIT 0x9000
#define CLR_BANKBIT 0x8000

#define CTRL_SET    0x1000
#define CTRL_CLR    0x0000

#define BONUSEN_BIT  0x2000
#define FLASHEN_BIT  0x4000
#define MAPRAMEN_BIT 0x4000
#define BANKSEL_BIT  0x8000
