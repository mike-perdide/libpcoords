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
 * $Id: debug.h 608 2009-05-24 21:43:48Z toady $
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <pcoords.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* level */
#define PICVIZ_DEBUG_NOTICE   0
#define PICVIZ_DEBUG_CRITICAL 1 /* defaults */
#define PICVIZ_DEBUG_WARNING  2
#define PICVIZ_DEBUG_DEBUG    3

/* area */
#define PICVIZ_AREA_CORE   0x0001
#define PICVIZ_AREA_PLUGIN 0x0002
#define PICVIZ_AREA_RENDER 0x0004
#define PICVIZ_AREA_PARSER 0x0008
#define PICVIZ_AREA_FILTER 0x0016

void pcoords_debug(int level, int area, const char *format, ...);
void pcoords_debug_print_axisplot(PcoordsAxisPlot *ap);

#ifdef __cplusplus
 }
#endif

#endif /* _DEBUG_H_ */
