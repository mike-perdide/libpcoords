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
 * $Id: py-picviz.c 644 2009-06-17 20:17:35Z toady $
 */

#include <Python.h>

#include <stdio.h>

#include <picviz.h>
#include <linuxlist.h>

#include "py-picviz.h"
#include "py-picviz_util.h"
#include "py-picviz_image.h"
#include "py-picviz_axes.h"
#include "py-picviz_lines.h"
#include "py-picviz_pgdl.h"

static int picviz_python_debug = 0;

PyObject *pypicviz_set_debug(PyObject *self, PyObject *args)
{
	PyObject *py_result = Py_BuildValue("d", 1);

	fprintf(stderr,"[PICVIZ_PYTHON_BINDINGS]: Debug mode activated\n");
	picviz_python_debug = 1;

	return py_result;
}

/**
 * ParseImage(filename, filter)
 * 'axes' -> ['axis1','axis2']
 * 'axis1' -> ['ymin','ymax']
 */
PyObject *pypicviz_image_new(PyObject *self, PyObject *args)
{
	PyObject *py_image_o; /* Core image structure */
        PyObject *main_dict;
        PyObject *axeslist;

	PicvizImage *image;
	PyObject *PyPicvizAxesObject;
	PyObject *PyPicvizLinesObject;

	int ret;
	unsigned int counter;
	char *filename;
	char *filter;


	/*
	 * We get the input variable (see function documentation above)
	 */
	if ( ! PyArg_ParseTuple(args, "sz", &filename, &filter) ) {
                PyErr_Print();
                PyErr_Clear();
                Py_RETURN_FALSE;
        }

	picviz_engine_init();
	picviz_init(NULL, NULL);
	main_dict = PyDict_New();
        axeslist = PyList_New(0);

	if (picviz_python_debug) {
		fprintf(stderr, "ParseImage name=[%s], filter=[%s]\n", filename, filter);
	}

	image = (PicvizImage *)pcv_parse(filename, filter);
	if ( ! image ) {
		fprintf(stderr, "Cannot create image\n");
		Py_RETURN_NONE;
	}

	if (picviz_python_debug) {
	        picviz_image_debug_printall(image);
	}

	ret = pypicviz_image_properties_set(main_dict, image);
	if (ret < 0) {
  	        fprintf(stderr, "Cannot set properties\n");
		Py_RETURN_NONE;
	}

        /* Give the axes list and their order */
	counter = 0;
	while (image->axesorder[counter]) {
		PyObject *axisname;
		
		axisname = Py_BuildValue("s", image->axesorder[counter]);
		PyList_Append(axeslist, axisname);
		Py_DECREF(axisname);
		counter++;
	}

        ret = PyDict_SetItemString(main_dict, "axeslist", axeslist);
	if (ret < 0) {
  	        fprintf(stderr, "Cannot set the axes list\n");
		Py_RETURN_NONE;
	}
	Py_DECREF(axeslist);

	PyPicvizAxesObject = pypicviz_build_axes(image);
        ret = PyDict_SetItemString(main_dict, "axes", PyPicvizAxesObject);
	if (ret < 0) {
  	        fprintf(stderr, "Cannot set the axes\n");
		Py_RETURN_NONE;
	}
	Py_DECREF(PyPicvizAxesObject);

	PyPicvizLinesObject = pypicviz_build_lines(image);
        ret = PyDict_SetItemString(main_dict, "lines", PyPicvizLinesObject);
	if (ret < 0) {
  	        fprintf(stderr, "Cannot set the lines\n");
		Py_RETURN_NONE;
	}
	Py_DECREF(PyPicvizLinesObject);

	py_image_o = PyCObject_FromVoidPtr((void *)image, NULL);
        ret = PyDict_SetItemString(main_dict, "image", py_image_o);
	if (ret < 0) {
  	        fprintf(stderr, "Cannot associate the image object to the main dictionary\n");
		Py_RETURN_NONE;
	}
	Py_DECREF(py_image_o);

	return main_dict;
}


/****************
 * Method table *
 ****************/
static PyMethodDef picvizMethods[] =
{
    { "Version", pypicviz_version_get, METH_VARARGS,
      "Returns the Picviz version"},
    { "Debug", pypicviz_set_debug, METH_VARARGS,
      "Set Picviz Python bindings in debug mode"},
    { "Image", pypicviz_image_new, METH_VARARGS,
      "Parse lines from the file"},
    { "setAxesOrder", pypicviz_axes_set_order, METH_VARARGS,
      "Set a new order for axes"},
    { "savePGDL", pypicviz_pgdl_save, METH_VARARGS,
      "Save the python structure into a PGDL file"},
    {NULL,NULL}
};


/* initpicviz = import picviz, not __init__() */
#ifdef WIN32
__declspec(dllexport)
#endif
void initpicviz(void)
{
 	PyObject *m, *d;


	m = Py_InitModule4("picviz",
			picvizMethods,
			"Parse, Render and Manipulate data from Picviz",
			0,
			PYTHON_API_VERSION);

	if ( ! m ) return;

	d = PyModule_GetDict(m);

}

