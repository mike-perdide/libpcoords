/*
 * Pcoords - Parallel coordinates ploter
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
 * $Id: values-mapping.h 326 2008-11-27 22:03:20Z toady $
 */

#ifndef _VALUES_MAPPING_H_
#define _VALUES_MAPPING_H_

#include <pcoords.h>

#ifdef __cplusplus
 extern "C" {
#endif

PcvHeight pcoords_line_value_get_from_string_dummy(PcoordsImage *image, PcoordsAxis *axis, int string_algo, PcvString string);
PcvHeight pcoords_line_value_get_with_minmax(PcoordsImage *image, PcoordsAxis *axis, char *string, PcvHeight min, PcvHeight max);
PcvHeight pcoords_values_mapping_get_from_y(PcoordsImage *image, PcvHeight max_val, PcvHeight y);

#ifdef __cplusplus
 }
#endif

#endif /* _VALUES_MAPPING_H_ */
