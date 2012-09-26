/*
 * Picviz - Parallel coordinates ploter
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

#include <picviz.h>


void output(PicvizImage *image, char *arg)
{
	PicvizLine *line;
	PicvizStats *stats;
	unsigned int counter;
	char *separator = ",";

	if (arg) separator = arg;

	llist_for_each_entry(line, &image->lines, list) {
	  counter = 0;
	  if (!line->hidden) {
	    while (image->axesorder[counter]) {
	      PicvizAxisPlot *axisplot = (PicvizAxisPlot *)picviz_hash_get(line->axesplots, image->axesorder[counter]);
	      PicvizAxis *axis = (PicvizAxis *)picviz_axis_get_from_name(image, image->axesorder[counter]);

	      fprintf(stderr,"name=%s,id=%llu\n",image->axesorder[counter], axis->id);

	      /* stats = (PicvizStats *)picviz_hash_get(axis->valcount, (char *)axisplot->strval); */
	      /* if ( ! stats) { */
	      /* 	      fprintf(stderr,"Cannot get the stats for %s from axis '%s'!\n", axisplot->strval, image->axesorder[counter]); */
	      /* 	      break; */
	      /* } else { */
	      /* 	      fprintf(stderr, "stat of %s is %llu", axisplot->strval, stats->counter); */
	      /* } */
	      /* if (!image->axesorder[counter+1]) { */
	      /* 	      fprintf(stdout, "\"%s\"[%llu]\n", axisplot->strval, stats->counter); */
	      /* } else { */
	      /* 	      fprintf(stdout, "\"%s\"[%llu]%s", axisplot->strval, stats->counter, separator); */
	      /* } */
	      counter++;
	    }
	  }
	}
}
