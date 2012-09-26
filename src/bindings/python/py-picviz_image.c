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
 * $Id: py-picviz_image.c 506 2009-04-18 08:11:31Z toady $
 */

#include <Python.h>

#include <picviz.h>

#include "py-picviz_util.h"
#include "py-picviz_image.h"

int pypicviz_image_properties_set(PyObject *main_dict, PicvizImage *image)
{
	int ret;

        ret = pypicviz_dict_keyval_append_long(main_dict, "height", image->height);
        if (ret < 0)
                return ret;

        ret = pypicviz_dict_keyval_append_long(main_dict, "width", image->width);
        if (ret < 0)
                return ret;

        ret = pypicviz_dict_keyval_append_str(main_dict, "title", image->title);
        if (ret < 0)
                return ret;

        ret = pypicviz_dict_keyval_append_str(main_dict, "bgcolor", image->bgcolor);
        if (ret < 0)
                return ret;

        ret = pypicviz_dict_keyval_append_str(main_dict, "logo", image->logo ? image->logo : "");
        if (ret < 0)
                return ret;

        ret = pypicviz_dict_keyval_append_long(main_dict, "logo_x", image->logo_x);
        if (ret < 0)
                return ret;

        ret = pypicviz_dict_keyval_append_long(main_dict, "logo_y", image->logo_y);
        if (ret < 0)
                return ret;

        ret = pypicviz_dict_keyval_append_str(main_dict, "filter", image->filter ? image->filter : "");
        if (ret < 0)
                return ret;

	return 0;
}
