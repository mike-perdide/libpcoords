/*
 * Pcoords - Parallel coordinates ploter
 * Copyright (C) 2009 Sebastien Tricaud <sebastien@honeynet.org>
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
 * $Id$
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pcoords.h>


static char *separator = ",";


void draw_line(PcoordsImage *image, PcvID axis_id, PcoordsLine *line, PcoordsAxisPlot *axisplot1, PcoordsAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2)
{

  char *frequency = pcoords_properties_get(axisplot1->props, "frequency");
  char *occurence = "1";
  char *source = axisplot1->strval;
  char *target = axisplot2->strval;

  fprintf(stdout, "\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"\n", frequency, separator, occurence, separator, source, separator, target);

}

void output(PcoordsImage *image, char *arg)
{

	PcoordsLine *line;

	if (arg) separator = arg;

	fprintf(stdout, "\"Frequency\"%s\"Occurence\"%s\"Source\"%s\"Target\"\n", separator, separator, separator);

	llist_for_each_entry(line, &image->lines, list) {
		if ( ! line->hidden ) {
			pcoords_line_draw(image, line, draw_line);
		}
	}

}

