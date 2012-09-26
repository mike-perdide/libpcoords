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
 * $Id: image.h 550 2009-05-03 11:45:09Z toady $
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "pcimage.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define IMAGE_Y_AS_RENDERED(image, y) \
   image->height - y

PicvizImage *picviz_image_new(void);
void picviz_image_destroy(PicvizImage *image);
void picviz_image_axis_append(PicvizImage *i, PicvizAxis *axis);
void picviz_image_line_append(PicvizImage *image, struct line_t *line);
void picviz_image_debug_printall(PicvizImage *i);

#ifdef __cplusplus
 }
#endif

#endif /* _IMAGE_H_ */
