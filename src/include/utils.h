/*
 * Pcoords - Parallel coordinates ploter
 * Copyright (C) 2008 Sebastien Tricaud <toady@gscore.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: utils.h 242 2008-10-15 17:33:26Z toady $
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <pcoords.h>

void pcoords_util_line_append(int x1, float y1, int x2, float y2);
int pcoords_util_line_exists(int x1, float y1, int x2, float y2);
char *pcoords_string_up(char *str);
int pcoords_is_string_algo_basic(PcoordsAxis *axis);
int pcoords_hex(unsigned char ch);
unsigned int pcoords_hex_to_int(char **ptr, int *intValue);

#endif /* _UTILS_H_ */
