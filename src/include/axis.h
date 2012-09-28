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
 * $Id: axis.h 601 2009-05-24 16:21:26Z toady $
 */

#ifndef _AXIS_H_
#define _AXIS_H_

#include <pcoords.h>

#include "pcimage.h"

#ifdef __cplusplus
 extern "C" {
#endif

struct axis_t *pcoords_axis_init(void);
void pcoords_axis_destroy(struct axis_t *axis);
struct axis_t *pcoords_axis_new(void);
void pcoords_axis_set_type_from_string(struct axis_t *axis, char *string);
char *pcoords_axis_get_string_from_type(PcoordsAxis *axis);
PcoordsAxis *pcoords_axis_get_from_name(PcoordsImage *image, PcvString name);
/* struct axis_t *pcoords_axis_get(PcoordsImage *i, unsigned int id); */
PcvWidth pcoords_axis_position_get(unsigned int counter);

#ifdef __cplusplus
 }
#endif

#endif /* _AXIS_H_ */

