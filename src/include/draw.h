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
 * $Id: draw.h 600 2009-05-24 16:04:57Z toady $
 */

#ifndef _DRAW_H_
#define _DRAW_H_

#include <picviz.h>
#include "line.h"
#include "pcimage.h"
#include "types.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef void (*PicvizDrawLine)(PicvizImage *image, PicvizLine *line, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2);

#ifdef __cplusplus
 }
#endif

#endif /* _DRAW_H_ */
