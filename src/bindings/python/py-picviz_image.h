/*
 * Picviz - Parallel coordinates ploter
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
 * $Id: py-picviz_image.h 505 2009-04-18 07:50:59Z toady $
 */

#ifndef _PY_PICVIZ_IMAGE_H_
#define _PY_PICVIZ_IMAGE_H_

#include <picviz.h>

#ifdef __cplusplus
 extern "C" {
#endif

int pypicviz_image_properties_set(PyObject *main_dict, PicvizImage *image);

#ifdef __cplusplus
 }
#endif

#endif /* _PY_PICVIZ_IMAGE_H_ */

