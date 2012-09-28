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
 * $Id: pcoords-pcre.h 148 2008-09-15 19:20:53Z toady $
 */

#ifndef _PICVIZ_PCRE_H_
#define _PICVIZ_PCRE_H_

#ifdef __cplusplus
 extern "C" {
#endif

PcoordsBool pcoords_regex_match(char *string, char *regex);

#ifdef __cplusplus
 }
#endif

#endif /* _PICVIZ_PCRE_H_ */
