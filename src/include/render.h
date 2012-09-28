/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2008 Sebastien Tricaud <toady@gscore.org>
 * Copyright (C) 2008 Philippe Saade <psaade@gmail.com>
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
 * $Id: render.h 556 2009-05-05 10:39:34Z toady $
 */

#ifndef _RENDER_H_
#define _RENDER_H_

//#include <pcimage.h>
#include <pcoords.h>

#ifdef __cplusplus
 extern "C" {
#endif

PicvizBool pcoords_axis_is_relative(PicvizAxis *axis);
void pcoords_render_line(PicvizImage *image, PicvizLine *line);
void pcoords_render_image(pcimage_t *image);
PcvHeight pcoords_render_value(PicvizImage *image, PicvizAxis *axis, PcvString value);
void pcoords_render_set_minmax(PicvizAxis *axis, PcvHeight value);

#ifdef __cplusplus
 }
#endif

#endif /* _RENDER_H_ */
