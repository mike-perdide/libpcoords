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
 * $Id: pcoords-init.h 600 2009-05-24 16:04:57Z toady $
 */

#ifndef _PICVIZ_INIT_H_
#define _PICVIZ_INIT_H_

#include <pcoords.h>

#ifdef __cplusplus
 extern "C" {
#endif

void pcoords_init(int *argc, char **argv);

#ifdef __cplusplus
 }
#endif

#endif /* _PICVIZ_INIT_H_ */
