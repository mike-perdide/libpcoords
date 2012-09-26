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
 * $Id: py-picviz_lines.c 553 2009-05-03 14:10:37Z toady $
 */

#include <Python.h>

#include <picviz.h>
#include <linuxlist.h>

#include "py-picviz_util.h"
#include "py-picviz_lines.h"

PyObject *linesdata;

static void draw_line(PicvizImage *image, PcvID axis_id, PicvizLine *line, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2)
{
	PyObject *PyPicvizLine = PyDict_New();
	int ret;

	/* We add the color information along with plots, to ease the line creation with QT */
	ret = pypicviz_dict_keyval_append_long(PyPicvizLine, "hidden", line->hidden);

	ret = pypicviz_dict_keyval_append_str(PyPicvizLine, "layer", line->layer ? line->layer : "");

	ret = pypicviz_dict_keyval_append_str(PyPicvizLine, "color", picviz_properties_get(line->props, "color"));

	ret = pypicviz_dict_keyval_append_float(PyPicvizLine, "penwidth", picviz_properties_get(line->props, "penwidth"));

	ret = pypicviz_dict_keyval_append_str(PyPicvizLine, "x1_strval", axisplot1->strval);

	ret = pypicviz_dict_keyval_append_str(PyPicvizLine, "x2_strval", axisplot2->strval);

	ret = pypicviz_dict_keyval_append_long(PyPicvizLine, "x1", x1);

	ret = pypicviz_dict_keyval_append_long(PyPicvizLine, "x2", x2);

	ret = pypicviz_dict_keyval_append_long(PyPicvizLine, "y1", IMAGE_Y_AS_RENDERED(image, y1));

	ret = pypicviz_dict_keyval_append_long(PyPicvizLine, "y2", IMAGE_Y_AS_RENDERED(image,y2));

	PyList_Append(linesdata, PyPicvizLine);
	Py_DECREF(PyPicvizLine);

}

PyObject *pypicviz_build_lines(PicvizImage *image)
{

	linesdata = PyList_New(0);
	PicvizLine *line;

	llist_for_each_entry(line, &image->lines, list) {
		picviz_line_draw(image, line, draw_line);
	}

	return linesdata;

}
