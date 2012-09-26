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
 * $Id: pcv-parser.h 353 2008-12-10 05:06:16Z toady $
 */

#ifndef _PCV_PARSER_H_
#define _PCV_PARSER_H_

#include <picviz.h>

#ifdef __cplusplus
 extern "C" {
#endif

PicvizImage *pcv_parse(char *filename, char *filter);
PicvizLine  *picviz_parse_line(char *string);

#ifdef __cplusplus
 }
#endif

#endif /* _PCV_PARSER_H_ */

