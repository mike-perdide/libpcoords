/*
 * Picviz - Parallel coordinates ploter
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
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <debug.h>
#include <linuxlist.h>
#include <plugins.h>
#include "axis.h"

#define FONT_SIZE_PER_CHAR 4

int get_x_from_string(int axis_x, char *str)
{
        int x;

        x = (strlen(str) * FONT_SIZE_PER_CHAR) / 2;

        return axis_x - x;
}

void draw_line(PicvizImage *image, PcvID axis_id, PicvizLine *line, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2)
{
	printf("Plot on the axis %llu the value %s at %llu with value %s at %llu\n", axis_id, axisplot1->strval, axisplot1->y, axisplot2->strval, axisplot2->y);
}

void output(pcimage_t *image, char *arg _U_)
{
        struct axis_t *a;
        struct line_t *l;
	unsigned int counter = 0;

	printf("*** This is the output plugin\n");
	printf("Image width:%d, height:%llu\n", image->width, image->height);
	printf("Header height:%llu\n", image->header_height);
	printf("bgcolor:%s\n", image->bgcolor);
	printf("logo:%s\n", image->logo);

	counter = 0;
	while (image->axesorder[counter]) {
		PicvizAxis *axis = (PicvizAxis *)pcoords_axis_get_from_name(image, image->axesorder[counter]);

		printf("id=%llu\n", axis->id);
		printf("name=%s\n", axis->name);
		printf("position=(counter=%d, engine.axis_x_cursor = %d) %d\n", counter, engine.axis_x_cursor, pcoords_axis_position_get(counter));

		counter++;
	}

        printf("Lines:\n");

        llist_for_each_entry(l, &image->lines, list) {
		counter = 0;
		printf("New line (color:%s)\n", pcoords_properties_get(l->props,"color"));
		if (!l->hidden) {
#if BOURIN
		while (image->axesorder[counter]) {
			PicvizAxisPlot *axisplot = (PicvizAxisPlot *)pcoords_hash_get(l->axesplots, image->axesorder[counter]);
                        struct axis_t *axis = (struct axis_t *)pcoords_axis_get_from_name(image, image->axesorder[counter]);

			printf("Plot on the axis position %d the value %s at %llu\n", pcoords_axis_position_get(counter), axisplot->strval, axisplot->y);
			counter++;
		}
#else
	pcoords_line_draw(image, l, draw_line);
#endif
		}
	}
        printf("EOF\n");
}


