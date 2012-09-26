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
 * $Id: py-picviz_util.h 532 2009-04-24 15:53:32Z toady $
 */

#ifndef _PY_PICVIZ_UTIL_H_
#define _PY_PICVIZ_UTIL_H_

#ifdef __cplusplus
 extern "C" {
#endif

PyObject *pypicviz_version_get(PyObject *self, PyObject *args);
int pypicviz_dict_keyval_append_str(PyObject *dict, char *key, char *val);
int pypicviz_dict_keyval_append_long(PyObject *dict, char *key, long val);
int pypicviz_dict_keyval_append_float(PyObject *dict, char *key, char *val);


#ifdef __cplusplus
 }
#endif

#endif /* _PY_PICVIZ_UTIL_H_ */

