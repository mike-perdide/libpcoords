/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2008-2009 Sebastien Tricaud <sebastien@honeynet.org>
 * Copyright (C) 2008 Philippe Saade <psaade@gmail.com>
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
 * $Id: render.c 682 2009-07-20 21:50:53Z toady $
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <picviz.h>
#include <linuxlist.h>

/**
 * \ingroup PicvizMain
 * @{
 */
/** \file render.c
 * \brief Apply rendering to the lines, including position and color
 */

/**
 * Finds wether we have lines that should be placed
 * relatively from each other or not on the axis
 *
 * @param axis the axis
 * @return TRUE if relative, FALSE otherwise
 */
PicvizBool picviz_axis_is_relative(PicvizAxis *axis)
{
        char *relative;
	if (!axis) return BOOL_FALSE;
        relative = picviz_properties_get(axis->props, "relative");
        if (relative) {
                if ( ! strcmp(relative, "true")) {
                        return BOOL_TRUE;
                }
        }

        return BOOL_FALSE;
}

static void picviz_render_set_image_width(PicvizImage *image)
{
	unsigned int counter = 0;

	while (image->axesorder[counter]) {
		image->width += engine.axis_default_space;
		counter++;
        }

}

void picviz_render_set_minmax(PicvizAxis *axis, PcvHeight value)
{

        if ( value < axis->ymin ) axis->ymin = value;
        if ( value > axis->ymax ) axis->ymax = value;

}

/**
 * Renders the value of a given string using the axis and image
 * properties.
 *
 * @param image the image
 * @param axis the axis
 * @param strvalue the string to extract the value from
 * @return the rendered value
 */
PcvHeight picviz_render_value(PicvizImage *image, PicvizAxis *axis, PcvString strvalue)
{
	PcvHeight strheight, maxval;
	PcvHeight retval = 0;
	float value;

	if ( ! axis ) {
		picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_RENDER, "No axis! cannot render anything.\n");
		exit(1);
	}

	/* XXX: This *must* be clean */
	if(axis->type == DATATYPE_ENUM) return;

	maxval = picviz_variable_max(image, axis, 0);
	strheight = picviz_line_value_get_from_string_dummy(image, axis, 1, strvalue);
	strheight -= axis->ymin;
	maxval = axis->ymax - axis->ymin;

	switch(axis->type) {
	case DATATYPE_PORT:
		if (strheight < 1024) {
			value = ((float)image->height / 2) / 1024;
			value *= strheight;
			retval = (PcvHeight) value;
		} else {
			value = ((float)(image->height - image->header_height) / 2) / (maxval - 1024);
			value *= strheight;
			retval = (PcvHeight) value + ((float)(image->height - image->header_height) / 2);
		}
		axis->render = PCV_RENDER_DONE;
		break;
	case DATATYPE_INTEGER:
	case DATATYPE_TIMELINE:
	case DATATYPE_SHORT:
		retval = picviz_values_mapping_get_from_y(image, maxval, strheight);
		axis->render = PCV_RENDER_DONE;
		break;
	case DATATYPE_NUMERIC:
		retval = (PcvHeight) picviz_values_mapping_get_from_y(image, axis->numeric, strheight);
		//fprintf(stderr, "maxval=%llu, retval=%llu\n", axis->numeric, retval);
		axis->render = PCV_RENDER_DONE;
		break;
	default:
		break;
	}

	return retval;
}

/**
 * Renders the image for each axis.
 *
 * @param image the image
 * @return the rendered value
 */
void picviz_render_image(PicvizImage *image)
{
        int ret;
        PicvizAxis *axis;
        struct line_t *line;
        struct axisplot_t *axisplot;
        PcvHeight strheight, maxval;
	/* XXX: Use axis->ymin/max instead */
        PcvHeight string_max[PICVIZ_MAX_AXES];
        int i = 1;
        int axis_position;
        int line_removal_candidate;
	unsigned int counter = 0;

	picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_RENDER, "Rendering");

        picviz_learn(image);


	picviz_render_set_image_width(image);

        if (! engine.__axis_label_exists) {
                image->header_height = 0;
        }

	counter = 0;
	while (image->axesorder[counter]) {
		PicvizAxis *axis = (PicvizAxis *)picviz_axis_get_from_name(image, image->axesorder[counter]);

		/* XXX: This is not just for strings but also for log type */
		/* if (!axis) continue; */
		if ((picviz_is_string_algo_basic(axis)) || axis->type == DATATYPE_LN) {
			string_max[i] = picviz_line_max_get(image, &image->lines, i);
			i++;
		}
		counter++;
        }

        /*
         * FIXME: ideally, this should be done while parsing...
         */
        llist_for_each_entry(line, &image->lines, list) {
		counter = 0;
		while (image->axesorder[counter]) {
			PicvizAxisPlot *axisplot = (PicvizAxisPlot *)picviz_hash_get(line->axesplots, image->axesorder[counter]);
			if ( ! axisplot ) {
				fprintf(stderr, "**FIXME** (Write a better error message): Cannot get the axis!\n");
				exit(1);
			}
                        /* PicvizAxis *axis = (PicvizAxis *)picviz_axis_get(image, axisplot->axis_id); */
			PicvizAxis *axis = (PicvizAxis *)picviz_axis_get_from_name(image, image->axesorder[counter]);
			if ( ! axis ) {
				fprintf(PICVIZ_DEBUG_WARNING, PICVIZ_AREA_RENDER, "Could not get the axis!");
				continue;
			}
			if (axis->render == PCV_RENDER_DONE) continue;
			if ( ! picviz_is_string_algo_basic(axis)) {
				if (picviz_axis_is_relative(axis)) {
					strheight = picviz_line_value_get_from_string_dummy(image, axis, 1, axisplot->strval);

					if ( strheight < axis->ymin )
						axis->ymin = strheight;

					if ( strheight > axis->ymax)
						axis->ymax = strheight;
				}
			}
			counter++;
                }
        }

	counter = 0;

	llist_for_each_entry(line, &image->lines, list) {
		axis_position = 0;
		counter = 0;
		line_removal_candidate = 0;
		PicvizAxisPlot *ap_tbl[PICVIZ_MAX_AXES];

		while (image->axesorder[counter]) {
			PicvizAxisPlot *axisplot = (PicvizAxisPlot *)picviz_hash_get(line->axesplots, image->axesorder[counter]);
			PicvizAxis *axis = (PicvizAxis *)picviz_axis_get_from_name(image, image->axesorder[counter]);

			if (!axis) continue;
			if (axis->render == PCV_RENDER_DONE) continue;

			if ( ( !(picviz_axis_is_relative(axis)) ) || (picviz_is_string_algo_basic(axis)) ) {
				strheight = picviz_line_value_get_from_string_dummy(image, axis, 0, axisplot->strval);
				maxval = picviz_variable_max(image, axis, 0);
				if (picviz_is_string_algo_basic(axis)) {
					if (axis->type == DATATYPE_STRING) {
						if (string_max[axisplot->axis_id] > picviz_variable_max(image, axis, 0)) {
							maxval = string_max[axisplot->axis_id];
						}
					}
				}
			} else {
				maxval = picviz_variable_max(image, axis, 1);
				strheight = picviz_line_value_get_from_string_dummy(image, axis, 1,axisplot->strval);
				strheight -= axis->ymin;
				maxval = axis->ymax - axis->ymin;
			}

			if (axis->type == DATATYPE_LN) {
				picviz_axisplot_set_y(axis, axisplot, 
						      picviz_line_value_get_with_minmax(image, axis, axisplot->strval, 0, string_max[axisplot->axis_id]));
			} else if (axis->type == DATATYPE_PORT) {
				float value;
				if (strheight < 1024) {
					value = ((float)image->height / 2) / 1024;
					value *= strheight;
					picviz_axisplot_set_y(axis, axisplot, 
							      (PcvHeight) value);
				} else {
					value = ((float)(image->height - image->header_height) / 2) / (maxval - 1024);
					value *= strheight;
					picviz_axisplot_set_y(axis, axisplot, 
							      (PcvHeight) value + ((float)(image->height - image->header_height) / 2));
				}
			} else {
				picviz_axisplot_set_y(axis, axisplot, 
						      picviz_values_mapping_get_from_y(image, maxval, strheight));
			}

			assert(axis_position < PICVIZ_MAX_AXES);
			ap_tbl[axis_position++] = axisplot;

			picviz_render_set_minmax(axis, axisplot->y);

			counter++;
		} /* while (image->axesorder[counter]) { */


		/* Filter */
                if ( image->filter ) {
                        ret = picviz_filter_display(image->filter, image, ap_tbl, axis_position);
                        if ( ret < 0 )
                                return;
                        line->hidden = ret;
                }

		/* Layers */
		if ((!line->hidden) && (line->layer)) {
		  char *layer;
		  layer = picviz_hash_get(image->hidden_layers, line->layer);
		  if (layer) line->hidden = 1;
		}
        }
}

/**
 * @}
 */
