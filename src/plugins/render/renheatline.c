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
 * $Id$
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <picviz.h>
#include <linuxlist.h>

PcvCounter most_frequent[PICVIZ_MAX_AXES]; /* Number holding the maximum line frequency */
PcvCounter most_frequent_virus; /* Number holding the maximum line frequency for virus mode */
PicvizCorrelation *pcvcor;

PicvizHLMode hlmode;

void find_most_frequent(PicvizImage *image _U_, PcvID axis_id, PicvizLine *line, PicvizAxisPlot *axisplot1 _U_, PicvizAxisPlot *axisplot2 _U_, PcvWidth x1 _U_, PcvHeight y1, PcvWidth x2 _U_, PcvHeight y2)
{
	PcvCounter counter;
	PcvString buffer[1024];
	PcvCounter i;

	if ( axis_id == 0 ) {
		for (i=0;i<=PICVIZ_MAX_AXES;i++) {
			most_frequent[i] = 0;
		}
	}

	snprintf((char *)buffer, 1024, "%llu:%llu,%llu",axis_id,y1,y2);
	counter = picviz_correlation_append(pcvcor, (char *)buffer);
	if (counter > most_frequent[axis_id]) {
		most_frequent[axis_id] = counter;
	}
	if (counter > most_frequent_virus) {
		most_frequent_virus = counter;
	}

	if (hlmode == VIRUS) {
		/* We finally kill the color */
		picviz_properties_set(line->props, "color","#000000");
	}

}

void redefine_colors_per_two_axes(PicvizImage *image, PcvID axis_id, PicvizLine *line, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2 _U_, PcvWidth x1 _U_, PcvHeight y1, PcvWidth x2 _U_, PcvHeight y2)
{
	PcvCounter counter;
	PcvString buffer[512];
	PcvString ratiobuf[512];
	PcvString color;
	double ratio;
	double ratio_tmp;
	char *frequency;
	int ret;

	snprintf((char *)buffer, 1024, "%llu:%llu,%llu",axis_id,y1,y2);
	counter = picviz_correlation_get(pcvcor, (char *)buffer);

	ratio_tmp = (double) (PcvCounter)counter / (PcvCounter)most_frequent[axis_id];
	ratio = ratio_tmp;
	snprintf((char *)ratiobuf, 512, "%f", ratio);
	picviz_properties_set(axisplot1->props, "frequency", (char *)ratiobuf);
	frequency = picviz_properties_get(axisplot1->props, "frequency");
	if (frequency) {
		ratio_tmp = strtod(frequency, (char **) NULL);
		if (ratio > ratio_tmp) {
			snprintf((char *)ratiobuf, 512, "%f", ratio);
			picviz_properties_set(axisplot1->props, "frequency", (char *)ratiobuf);
			ratio = ratio_tmp;
		}
	}

	if (engine.debug) {
		fprintf(stdout,"RENDER:ratio=%f;counter=%llu,most_frequent[axis_id]=%llu\n",ratio,counter,most_frequent[axis_id]);
	}
	frequency = picviz_properties_get(axisplot1->props, "frequency");
	if (frequency) {
		ratio = strtod(frequency, (char **) NULL);

                if ( image->filter ) {
		  if (!line->hidden) { /* if it has been filtered, we leave it filtered */
                        ret = picviz_filter_renplugin(image->filter, image, frequency, line);
                        if ( ret < 0 )
                                return;

/* 			fprintf(stderr, "ret=%d,freq=%s\n", ret, frequency); */

                        line->hidden = ret;
		  }
                }
		
/* 		fprintf(stderr, "ratio = %f\n", ratio); */
		color = picviz_correlation_heatline_get(ratio);
		if (engine.debug) {
			fprintf(stdout, "RENDER:We can set the color '%s' on line (id=%llu,axis:%llu;y1:%llu;y2:%llu)\n", color, line->id, axis_id, y1, y2);
		}

		picviz_properties_set(axisplot1->props, "color", color);
	}
}

void redefine_colors_virus(PicvizImage *image _U_, PcvID axis_id, PicvizLine *line, PicvizAxisPlot *axisplot1 _U_, PicvizAxisPlot *axisplot2 _U_, PcvWidth x1 _U_, PcvHeight y1, PcvWidth x2 _U_, PcvHeight y2)
{
	PcvCounter counter;
	PcvString buffer[1024];
	PcvString ratiobuf[512];
	PcvString color;
	double ratio;
	double ratio_tmp;

	snprintf((char *)buffer, 1024, "%llu:%llu,%llu",axis_id,y1,y2);
	counter = picviz_correlation_get(pcvcor, (char*)buffer);

	ratio_tmp = (double) (PcvCounter)counter / (PcvCounter)most_frequent_virus;
	ratio = ratio_tmp;
	if (axis_id == 1) {
		snprintf((char *)ratiobuf, 512, "%f", ratio);
		picviz_properties_set(line->props, "frequency", (char *)ratiobuf);
	} else {
		char *frequency = picviz_properties_get(line->props, "frequency");
		ratio_tmp = strtod(frequency, (char **) NULL);
		if (ratio > ratio_tmp) {
			snprintf((char *)ratiobuf, 512, "%f", ratio);
			picviz_properties_set(line->props, "frequency", (char *)ratiobuf);
			ratio = ratio_tmp;
		}
	}
	if (engine.debug) {
		fprintf(stdout,"RENDER:ratio=%f;counter=%llu,most_frequent=%llu\n",ratio,counter,most_frequent_virus);
	}
	ratio = strtod(picviz_properties_get(line->props, "frequency"), (char **) NULL);
	color = picviz_correlation_heatline_get(ratio);
	if (engine.debug) {
		fprintf(stdout, "RENDER:We can set the color '%s' on line (id=%llu,axis:%llu;y1:%llu;y2:%llu)\n", color, line->id, axis_id, y1, y2);
	}

	picviz_properties_set(line->props, "color", color);
}

void debug_colors(PicvizImage *image _U_, PcvID axis_id, PicvizLine *line _U_, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2 _U_, PcvWidth x1 _U_, PcvHeight y1 _U_, PcvWidth x2 _U_, PcvHeight y2 _U_)
{
	printf("RENDER:Axis id=%llu, color=%s\n", axis_id, picviz_properties_get(axisplot1->props, "color"));
}

void render(pcimage_t *image, char *arg)
{
	struct line_t *line;

	fprintf(stderr, "[+] Render heatline plugin\n");

	engine.draw_heatline = 1;
	engine.debug = 0;

	picviz_correlation_new(&pcvcor);

	if (!arg) {
		hlmode = PER_TWO_AXES; /* Default */
		engine.heatline_algo = 0;
	} else {
		if ( ! strcmp(arg,"virus") ) {
			hlmode = VIRUS;
			engine.heatline_algo = 1;
		}
		if ( ! strcmp(arg,"origin") ) {
			hlmode = FROM_POINT;
			engine.heatline_algo = 2;
		}
		if ( ! strcmp(arg,"full") ) {
			hlmode = PER_FULL_LINE;
			engine.heatline_algo = 3;
		}
	}


	/*
	 * #1: We get the most frequent line and we build hashes
	 */
	llist_for_each_entry(line, &image->lines, list) {
		picviz_line_draw(image, line, find_most_frequent);
	}
	/*
	 * #2: We build line colors
	 */
	if ( hlmode == PER_TWO_AXES ) {
		llist_for_each_entry(line, &image->lines, list) {
			picviz_line_draw(image, line, redefine_colors_per_two_axes);
		}
	}
	if ( hlmode == VIRUS ) {
		llist_for_each_entry(line, &image->lines, list) {
			picviz_line_draw(image, line, redefine_colors_virus);
		}
	}

	if (engine.debug) {
		llist_for_each_entry(line, &image->lines, list) {
			picviz_line_draw(image, line, debug_colors);
		}
	}

}

