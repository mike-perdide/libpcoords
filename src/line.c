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
 * $Id: line.c 657 2009-06-26 18:45:52Z toady $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcoords.h>

static PcvID id = 0;


PicvizLine *pcoords_line_new(void)
{
        PicvizLine *line;

        line = malloc(sizeof(*line));
        if ( ! line ) {
                fprintf(stderr, "Cannot initialize line: memory exhausted.\n");
                return NULL;
        }
        INIT_LLIST_HEAD(&line->list);
        line->id = id++;
        line->hidden = 0;
        line->layer = NULL;

	pcoords_hash_new(&line->axesplots);
        pcoords_properties_new(&line->props);
        pcoords_properties_set(line->props, "color", "#000000");
        pcoords_properties_set(line->props, "penwidth", DEFAULT_PENWIDTH);

        return line;
}

void pcoords_line_axis_append(PicvizLine *line, PcvString axisname, PicvizAxisPlot *axisplot)
{
	pcoords_hash_set(line->axesplots, axisname, axisplot, sizeof(*axisplot));
}

void pcoords_line_axis_foreach(PicvizImage *image, PicvizLine *line,
			      void (*each_axis_function)(PcvString key, PicvizAxisPlot *axisplot))
{
	unsigned int counter = 0;

	while (image->axesorder[counter]) {
		PicvizAxisPlot *ap;
		ap = (PicvizAxisPlot *)pcoords_hash_get(line->axesplots, image->axesorder[counter]);
		each_axis_function(image->axesorder[counter], ap);
		counter++;
	}
}






void pcoords_line_axis_foreach_unique(PicvizImage *image, PicvizLine *line,
			      void (*each_axis_function)(PcvString key, PicvizAxisPlot *axisplot))
{
	unsigned int counter = 0;
	unsigned int s_counter = 0; /* Counter for storage */
	PcvString axesstorage[PICVIZ_MAX_AXES+1];

	memcpy(axesstorage, image->axesorder, sizeof(image->axesorder));

	while (image->axesorder[counter]) {
		s_counter = 0;
		while (axesstorage[s_counter]) {
			if (!strcmp(axesstorage[s_counter], image->axesorder[counter])) {
			/*
			 * The trick is to replace existing values with a '*' to know we have a duplicate
			 */
/* 				free(axesstorage[s_counter]); */
				axesstorage[s_counter] = strdup("*");
			}
			s_counter++;
		}

		counter++;
	}

	counter = 0;
	while (axesstorage[counter]) {
		PicvizAxisPlot *ap;
		if (strcmp(axesstorage[counter], "*")) {
			ap = (PicvizAxisPlot *)pcoords_hash_get(line->axesplots, image->axesorder[counter]);
			each_axis_function(image->axesorder[counter], ap);
		}
		counter++;
	}
}


static void _pcoords_line_axisplot_free(PcvString key, PicvizAxisPlot *axisplot)
{
	if (axisplot) {
		free(axisplot);
	}
}

void pcoords_line_free(PicvizImage *image, PicvizLine *line)
{
        pcoords_properties_destroy(line->props);
	pcoords_line_axis_foreach_unique(image, line,
					_pcoords_line_axisplot_free);
        free(line);

}

PcvHeight pcoords_line_max_get(PicvizImage *image, struct llist_head *line, PcvID axis_id)
{
	PicvizLine *l;
	PicvizAxisPlot *axisplot;
	char init = 0;
	PcvHeight max = 0;
	PcvHeight height = 0;
	unsigned int counter = 0;

//	printf("We should return the max value if the axis %d\n", axis_id);

	llist_for_each_entry(l, line, list) {
		counter = 0;
		while (image->axesorder[counter]) {
			PicvizAxis *axis = (PicvizAxis *)pcoords_axis_get_from_name(image, image->axesorder[counter]);
			PicvizAxisPlot *axisplot = (PicvizAxisPlot *)pcoords_hash_get(l->axesplots, image->axesorder[counter]);
			//printf("Y VAL=%lld\n", axisplot->y);
			if (pcoords_is_string_algo_basic(axis)) {
				height = pcoords_line_value_get_from_string_dummy(image, axis, 0, axisplot->strval);
			} else {
				height = pcoords_line_value_get_from_string_dummy(image, axis, 1, axisplot->strval);
			}
			if (axis_id == axisplot->axis_id) {
				if ( ! init ) {
					//					printf("INIT\n");
					init = 1;
					max = height;
				}

				//				printf("max=%lld, axisplot->y =%lld\n", max, height);

				if ( height > max ) max = height;
			}

			counter++;
		}
	}

	return max;
}

PcvHeight pcoords_line_max_pertype_get(PicvizImage *image, PicvizDataType type)
{
        PicvizLine *line;
        int i;
        int nb_types = sizeof(PicvizDataType);
        char init[nb_types];
        PcvHeight max[nb_types];
	unsigned int counter = 0;

        for ( i = 0; i <= nb_types; i++) {
                init[i] = 0;
        }

        llist_for_each_entry(line, &image->lines, list) {
		counter = 0;
		while (image->axesorder[counter]) {
			PicvizAxisPlot *axisplot = (PicvizAxisPlot *)pcoords_hash_get(line->axesplots, image->axesorder[counter]);
                        PicvizAxis *axis = (PicvizAxis *)pcoords_axis_get_from_name(image, image->axesorder[counter]);

                        if (axis->type == type) {
                                if ( ! init[type] ) {
                                        init[type] = 1;
                                        max[type] = axisplot->y;
                                }

                                if ( axisplot->y > max[type] ) max[type] = axisplot->y;

                        }

			counter++;
		}

        }

        return max[type];
}

/* Sets the callback to draw */
int pcoords_line_draw(PicvizImage *image, PicvizLine *line, void (*draw_line_func)(PicvizImage *image, PcvID count, PicvizLine *line, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2))
{
	PicvizAxisPlot *last_axisplot;
	PcvWidth  last_x;
	PcvHeight last_y;
	unsigned int counter = 0;

	while (image->axesorder[counter]) {
	  PicvizAxisPlot *axisplot;
	  PicvizAxis *axis;

/* 	  fprintf(stderr, "PLD: %s:%d\n", image->axesorder[counter], counter); */

	  axisplot = (PicvizAxisPlot *)pcoords_hash_get(line->axesplots, image->axesorder[counter]);
	  axis = (PicvizAxis *)pcoords_axis_get_from_name(image, image->axesorder[counter]);

		if (!axis) {
			fprintf(stderr, "[libpcoords] Error: no axis named '%s'\n", image->axesorder[counter]);
			return 0;
		}

		if (counter == 0) {
			last_y = image->height - axisplot->y;
			/* last_x = axis->xpos; */
			last_x = pcoords_axis_position_get(counter);
			last_axisplot = axisplot;
		} else {
			draw_line_func(image, counter, line, last_axisplot, axisplot, last_x, last_y, pcoords_axis_position_get(counter), image->height - axisplot->y);
			/* last_x = axis->xpos; */
			last_x = pcoords_axis_position_get(counter);
			last_y = image->height - axisplot->y;
			last_axisplot = axisplot;
		}

		counter++;
	}

	return 0;
}

PicvizLine *pcoords_line_id_get(PicvizImage *image, PcvID line_id)
{
	PicvizLine *line;

	llist_for_each_entry(line, &image->lines, list) {
		if (line->id == line_id) return line;
	}

	return NULL;
}

void pcoords_line_debug(PicvizLine *line)
{
	fprintf(stdout, "line->id=%llu\n", line->id);
	fprintf(stdout, "line->hidden=%d\n", line->hidden);
}

unsigned int pcoords_line_all_axisplot_get(PicvizImage *image, PicvizLine *line, PicvizAxisPlot **ap)
{
        unsigned int counter;
	char *freq;

	counter = 0;
	while (image->axesorder[counter]) {
	        PicvizAxisPlot *axisplot = (PicvizAxisPlot *)pcoords_hash_get(line->axesplots, image->axesorder[counter]);
		ap[counter] = axisplot;
		counter++;
	}

	return counter;
}

void pcoords_line_draw_text(PicvizImage *image, LinePrintTextFlags flags, PcvID axis_id, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2, void (*draw_text)(double x, double y, char *color, double size, char *text))
{
	/* Print values on axes, dealing with the print property one can give to an axis */
	if (engine.display_raw_data) {
		PicvizAxis *axis = pcoords_axis_get_from_name(image, image->axesorder[axis_id]);
		char *print = pcoords_properties_get(axis->props, "print");
		char font_size;

		font_size = image->font_size ? image->font_size :
		                               image->height / engine.font_factor;

		if (!print) print = "true";
		if (axis_id == 1) {
			/* This is the first line, so we draw texts at origin AND destination */
			PicvizAxis *axis0 = pcoords_axis_get_from_name(image, image->axesorder[0]);
			char *print0 = pcoords_properties_get(axis0->props, "print");
			if (!print0) print0 = "true";

			if (strcmp(print0,"false")) {
			        if (flags &= LPF_MINMAXONLY) {
			                if ((IMAGE_Y_AS_RENDERED(image, axis0->ymin) == y1) ||
					    (IMAGE_Y_AS_RENDERED(image, axis0->ymax) == y1)) {
					        draw_text(x1, y1,
							  image->font_color,
							  font_size,
							  axisplot1->strval);
					}
				} else {
				        draw_text(x1, y1,
					          image->font_color,
						  font_size,
						  axisplot1->strval);
				}
			}
		}
		if (strcmp(print,"false")) {
		        if (flags &= LPF_MINMAXONLY) {
			        if ((IMAGE_Y_AS_RENDERED(image, axis->ymin) == y2) ||
				    (IMAGE_Y_AS_RENDERED(image, axis->ymax) == y2)) {
				        draw_text(x2, y2,
					          image->font_color,
						  font_size,
						  axisplot2->strval);
				}
			} else {
			        draw_text(x2, y2,
					  image->font_color,
					  font_size,
					  axisplot2->strval);
			}
		}
	}

}

