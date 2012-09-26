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
 * $Id: py-picviz_axes.c 618 2009-05-27 17:38:19Z toady $
 */

#include <Python.h>

#include <picviz.h>

#include "py-picviz_util.h"
#include "py-picviz_lines.h"
#include "py-picviz_axes.h"

PyObject *pypicviz_build_axes(PicvizImage *image)
{
	PyObject *axesdata = PyDict_New();
	int ret;
	unsigned int counter = 0;

	while (image->axesorder[counter]) {
		PicvizAxis *axis = (PicvizAxis *)picviz_axis_get_from_name(image, image->axesorder[counter]);
		PyObject *axisdata = PyDict_New();

		ret = pypicviz_dict_keyval_append_long(axisdata, "id", axis->id);
                if (ret < 0) {
                        Py_RETURN_NONE;
                }
		ret = pypicviz_dict_keyval_append_str(axisdata, "type", picviz_axis_get_string_from_type(axis));
                if (ret < 0) {
                        Py_RETURN_NONE;
                }
		ret = pypicviz_dict_keyval_append_str(axisdata, "label", picviz_properties_get(axis->props, "label"));
                if (ret < 0) {
                        Py_RETURN_NONE;
                }
		
		ret = PyDict_SetItemString(axesdata, image->axesorder[counter], axisdata);
		Py_DECREF(axisdata);

		counter++;
	}

	return axesdata;
}

/* setAxesOrder(image, ['axis1','axis2','axis2']) */
PyObject *pypicviz_axes_set_order(PyObject *self, PyObject *args)
{
	PicvizImage *image;
	PyObject    *image_o;       /* 1st parameter */
	PyObject    *image_pcvimage_o;    /* image['image'] */
	PyObject    *new_axesorder; /* 2nd parameter: ['axis1','axis2'...] */
	PyObject    *axisname_o; /* What we get while browsing the 2nd parameter */
	PyObject    *PyPicvizLinesObject;
	char        *axisname;
	unsigned int counter = 0;
	size_t axeslist_size;
	int ret;

	if ( ! PyArg_ParseTuple(args, "OO", &image_o, &new_axesorder)) {
		Py_RETURN_FALSE;
	}

	image_pcvimage_o = PyDict_GetItemString(image_o, "image");
	if (!image_pcvimage_o) {
		Py_RETURN_FALSE;
	}
	axeslist_size = PyList_GET_SIZE(new_axesorder);

 	image = (PicvizImage *)PyCObject_AsVoidPtr(image_pcvimage_o);
	if (!image) {
 	        fprintf(stderr, "Error: no image from C object");
		Py_RETURN_FALSE;
	}

	while (image->axesorder[counter]) {
		free(image->axesorder[counter]);
		image->axesorder[counter] = NULL;
		counter++;
	}

	counter = 0;
	while (counter < axeslist_size) {
		axisname_o = PyList_GET_ITEM(new_axesorder, counter);
		assert(axisname_o);
		if PyString_Check(axisname_o) {
			Py_INCREF(axisname_o);
			axisname = PyString_AS_STRING(axisname_o);
			image->axesorder[counter] = strdup(axisname);
		}
		counter++;
	}

	image->axesorder[counter] = NULL;

	PyPicvizLinesObject = pypicviz_build_lines(image);
	
        ret = PyDict_SetItemString(image_o, "lines", PyPicvizLinesObject);
	if (ret < 0) {
		Py_RETURN_FALSE;
	}
	Py_DECREF(PyPicvizLinesObject);

	Py_RETURN_TRUE;
}

