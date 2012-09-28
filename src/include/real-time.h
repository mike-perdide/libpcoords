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
 * $Id: real-time.h 565 2009-05-06 19:17:19Z toady $
 */

#ifndef _REAL_TIME_H_
#define _REAL_TIME_H_


#ifdef __cplusplus
 extern "C" {
#endif

int pcoords_fifo_data_read(PicvizImage *template, char *filename, void (*fifo_cb)(PicvizImage *image));

#ifdef __cplusplus
 }
#endif

#endif /* _REAL_TIME_H_ */

