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
 * $Id: py-picviz_util.c 541 2009-04-29 14:47:48Z toady $
 */

#include <Python.h>

#include "py-picviz.h"
#include "py-picviz_util.h"


PyObject *pypicviz_version_get(PyObject *self, PyObject *args)
{
	PyObject *py_result = Py_BuildValue("s", PICVIZ_PYTHON_VERSION);
	return py_result;
}

int pypicviz_dict_keyval_append_str(PyObject *dict, char *key, char *val)
{
        PyObject *value;
        int ret;

        value = PyString_FromString(val);

        ret = PyDict_SetItemString(dict, key, value);
        if (ret < 0) {
                fprintf(stderr, "[PYTHON_BINDINGS]: Can not append value '%s' to '%s'", val, key);
        }

        return ret;
}

int pypicviz_dict_keyval_append_long(PyObject *dict, char *key, long val)
{
        PyObject *value;
        int ret;

        value = PyInt_FromLong(val);

        ret = PyDict_SetItemString(dict, key, value);
        if (ret < 0) {
                fprintf(stderr, "[PYTHON_BINDINGS]: Can not append value '%li' to '%s'", val, key);
        }

        return ret;
}

int pypicviz_dict_keyval_append_float(PyObject *dict, char *key, char *val)
{
        PyObject *value;
        int ret;

        value = PyFloat_FromString(PyString_FromString(val), NULL); /* second arg. ignored, only for backward compat. */

        ret = PyDict_SetItemString(dict, key, value);
        if (ret < 0) {
                fprintf(stderr, "[PYTHON_BINDINGS]: Can not append value '%s' to '%s'", val, key);
        }

        return ret;
}
