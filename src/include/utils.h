/*
 * Picviz - Parallel coordinates ploter
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

#include <picviz.h>

void picviz_util_line_append(int x1, float y1, int x2, float y2);
int picviz_util_line_exists(int x1, float y1, int x2, float y2);
char *picviz_string_up(char *str);
int picviz_is_string_algo_basic(PicvizAxis *axis);
int picviz_hex(unsigned char ch);
unsigned int picviz_hex_to_int(char **ptr, int *intValue);

#endif /* _UTILS_H_ */
