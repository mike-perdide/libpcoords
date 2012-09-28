/*
 * Pcoords - Parallel coordinates ploter
 * Copyright (C) 2008-2009 Sebastien Tricaud <sebastien@honeynet.org>
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
 * $Id$
 */

#ifndef _AXISPLOT_H_
#define _AXISPLOT_H_

#include <pcoords.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct axisplot_t *pcoords_axisplot_new(void);
void pcoords_axisplot_set_y(PcoordsAxis *axis, PcoordsAxisPlot *axisplot, PcvHeight y);
void pcoords_axisplot_destroy(PcoordsAxisPlot *axisplot);

#ifdef __cplusplus
 }
#endif

#endif /* _AXISPLOT_H_ */
