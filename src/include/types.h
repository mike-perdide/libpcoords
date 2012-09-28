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
 * $Id: types.h 195 2008-09-26 07:26:09Z toady $
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define _U_ __attribute__((unused))

typedef enum pcoords_bool_t {
	BOOL_ERROR = -1,
	BOOL_FALSE = 0,
	BOOL_TRUE  = 1
} pcoords_bool_t;
typedef enum pcoords_bool_t PicvizBool;

typedef unsigned long long PcvHeight;
typedef unsigned int  PcvWidth;
typedef unsigned long long PcvID;
typedef unsigned long long PcvCounter;
typedef char *        PcvString;

#define PCVSTRING_TO_C(x) x;
#define PICVIZ_MAX_AXES 1024

#ifdef __cplusplus
 }
#endif

#endif /* _TYPES_H_ */

