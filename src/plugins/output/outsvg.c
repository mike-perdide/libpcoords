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
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <pcoords.h>
#include <linuxlist.h>

#define FONT_SIZE_PER_CHAR 4

FILE *fd;

int get_x_from_string(int axis_x, char *str)
{
	int x;

	x = (strlen(str) * FONT_SIZE_PER_CHAR) / 2;

	return axis_x - x;
}

void pcoords_svg_printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(fd, format, ap);
	va_end(ap);

	fflush(fd);

}

void debug_colors(PcoordsImage *image _U_, PcvID axis_id, PcoordsLine *line _U_, PcoordsAxisPlot *axisplot1, PcoordsAxisPlot *axisplot2 _U_, PcvWidth x1 _U_, PcvHeight y1 _U_, PcvWidth x2 _U_, PcvHeight y2 _U_)
{
	printf("OUTPUT:Axis id=%llu, color=%s\n", axis_id, pcoords_properties_get(axisplot1->props, "color"));
}

static void draw_text(double x, double y, char *color, double size, char *text)
{
	pcoords_svg_printf("<text style=\"fill:black;font-size:%fpx;font-family:Courier 10 Pitch\" x=\"%f\" y=\"%f\">%s</text>\n",
			  size, x, y, text);
}

void draw_line(PcoordsImage *image, PcvID axis_id, PcoordsLine *line, PcoordsAxisPlot *axisplot1, PcoordsAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2)
{
        LinePrintTextFlags lpflags;

	pcoords_svg_printf("<line x1=\"%d\" y1=\"%llu\" x2=\"%d\" y2=\"%llu\" ",
			  x1, y1, x2, y2);
	if ( ! engine.draw_heatline ) {
		pcoords_svg_printf("stroke=\"%s\" stroke-width=\"%s\"/>\n",
				  pcoords_properties_get(line->props, "color"), pcoords_properties_get(line->props, "penwidth"));
	} else {
		switch (engine.heatline_algo) {
		case 0: /* Default */
			pcoords_svg_printf("stroke=\"%s\" stroke-width=\"%s\"/>\n",
					  pcoords_properties_get(axisplot1->props, "color"),
					  pcoords_properties_get(line->props, "penwidth"));
			break;
		case 1: /* Virus */
			pcoords_svg_printf("stroke=\"%s\" stroke-width=\"%s\"/>\n",
					  pcoords_properties_get(line->props, "color"),
					  pcoords_properties_get(line->props, "penwidth"));
			break;
		}
	}

        lpflags = LPF_DEFAULTS; 
        if (engine.display_minmax) lpflags = LPF_MINMAXONLY;
        pcoords_line_draw_text(image, lpflags, axis_id, x1, y1, x2, y2, axisplot1, axisplot2, draw_text);

}

void output(pcimage_t *image, char *arg _U_)
{
	PcoordsLine *line;
	const char *label;
	unsigned int counter;

	if (engine.output_file) {
		fd = fopen(engine.output_file, "w");
		fprintf(stderr, "[+] Output pngcairo image to '%s'\n", engine.output_file);
	} else {
		fd = stdout;
		fprintf(stderr, "[+] Output pngcairo image to stdout\n");
	}

	pcoords_svg_printf("<?xml version=\"1.0\" standalone=\"no\"?>\n");
	pcoords_svg_printf("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	pcoords_svg_printf("<svg width=\"%d\" height=\"%llu\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n",
			image->width, image->height);
	pcoords_svg_printf("  <desc>Pcoords image</desc>\n\n");

	pcoords_svg_printf("/* Background */\n");
	pcoords_svg_printf("<rect id=\"square\" style=\"fill:white;stroke:white;\" width=\"%d\" height=\"%llu\"></rect>\n\n", image->width, image->height);

	if (image->logo) {
		pcoords_svg_printf("<image\n");
		pcoords_svg_printf("x=\"%d\" y=\"%llu\"\n", image->logo_x, image->logo_y);
		pcoords_svg_printf("id=\"logo\"\n");
		pcoords_svg_printf("xlink:href=\"%s\" />\n", image->logo);
	}

	pcoords_svg_printf("/* Axes */\n");

	counter = 0;
	while (image->axesorder[counter]) {
	        PcoordsAxis *axis = (PcoordsAxis *)pcoords_axis_get_from_name(image, image->axesorder[counter]);

		label = pcoords_properties_get(axis->props, "label");
		pcoords_svg_printf("<line x1=\"%d\" y1=\"%llu\" x2=\"%d\" y2=\"%llu\" stroke=\"black\" stroke-width=\"0.1\"/>\n",
				pcoords_axis_position_get(counter), image->header_height, pcoords_axis_position_get(counter), image->height);
		pcoords_svg_printf("<text style=\"fill:black;font-size:8px;font-family:Courier 10 Pitch\" x=\"%d\" y=\"10\">%s</text>\n", get_x_from_string(pcoords_axis_position_get(counter), (char *)label), label);
		pcoords_svg_printf("<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%llu\" stroke=\"black\" stroke-width=\"0.1\"/>\n", pcoords_axis_position_get(counter) + (engine.axis_default_space/2), 0, pcoords_axis_position_get(counter) + (engine.axis_default_space/2), image->header_height);

		counter++;
	}

	pcoords_svg_printf("<line x1=\"0\" y1=\"%llu\" x2=\"%d\" y2=\"%llu\" stroke=\"black\" stroke-width=\"0.1\"/>\n", image->header_height, image->width, image->header_height);
	pcoords_svg_printf("\n/* Lines */\n");

	llist_for_each_entry(line, &image->lines, list) {
		if (!line->hidden) {
			pcoords_line_draw(image, line, draw_line);
		}
	}
	pcoords_svg_printf("</svg>\n");

	/* if (engine.debug) { */
	/* 	llist_for_each_entry(l, &image->lines, list) { */
	/* 		pcoords_line_draw(image, l, debug_colors); */
	/* 	} */
	/* } */

	if (engine.output_file) {
		fclose(fd);
	}
}


