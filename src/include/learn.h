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
 * $Id: learn.h 599 2009-05-24 14:12:05Z toady $
 */

#ifndef _LEARN_H_
#define _LEARN_H_

#include <pcoords.h>

#ifdef __cplusplus
 extern "C" {
#endif

void pcoords_learn(PicvizImage *image);

#ifdef __cplusplus
 }
#endif

#endif /* _LEARN_H_ */
