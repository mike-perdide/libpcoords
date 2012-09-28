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

void render(pcimage_t *image, char *arg _U_)
{
        struct axis_t *a;
        struct line_t *l;
        struct axisplot_t *axisplot;

	printf("**** This is the rendering plugin\n");
	printf("Image width:%d, height:%llu\n", image->width, image->height);
	printf("Header height:%llu\n", image->header_height);

	printf("Axes:\n");
        llist_for_each_entry(a, &image->axes, list) {
		printf("xpos=%d\n", a->xpos);
        }
	printf("\n");

        printf("Lines:\n");

        llist_for_each_entry(l, &image->lines, list) {
		printf("New line\n");
		if (l->hidden) {
			printf("This line is hidden\n");
		}
                llist_for_each_entry(axisplot, &l->axisplot, list) {
                        struct axis_t *axis = (struct axis_t *)pcoords_axis_get(image, axisplot->axis_id);

			printf("Plot on the axis position %d the value %s at %llu\n", axis->xpos, axisplot->strval, axisplot->y);
        	}
	}
        printf("EOF\n");
}


