/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2008 Sebastien Tricaud <toady@gscore.org>
 *
 * Simple output plugin demo
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <debug.h>
#include <linuxlist.h>
#include <plugins.h>
#include "axis.h"


void draw_line(PicvizImage *image, PcvID axis_id, PicvizLine *line, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2)
{
	printf("OUTPUT:Axis id=%llu, color=%s\n", axis_id, pcoords_properties_get(axisplot1->props, "color"));
}


void output(const pcimage_t *image, char *arg _U_)
{
	PicvizLine *line;

	llist_for_each_entry(line, &image->lines, list) {
		pcoords_line_draw(image, line, draw_line);
	}
}


