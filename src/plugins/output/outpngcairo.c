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
#include <cairo.h>


cairo_t *cr;

/* To draw the text every N lines (pcv -LN) */
PcvCounter draw_rawtext[PICVIZ_MAX_AXES];
char *parameter = "";

static void draw_text(double x, double y, char *color, double size, char *text)
{
	cairo_set_source_rgb(cr,
			     picviz_color_extract_r(color),
			     picviz_color_extract_g(color),
			     picviz_color_extract_b(color));

	cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, size);

	cairo_move_to (cr, x, y);
	cairo_show_text (cr, text);
	
	cairo_stroke(cr);
}

void draw_line(PicvizImage *image, PcvID axis_id, PicvizLine *line, PicvizAxisPlot *axisplot1, PicvizAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2)
{
	char *color = NULL;
	double line_width;
	LinePrintTextFlags lpflags;

	switch(engine.heatline_algo) {
		case 0: /* Default */
			color = picviz_properties_get(axisplot1->props, "color");
			break;
		case 1: /* Virus */
			color = picviz_properties_get(line->props, "color");
			break;
	}

	if (!color) color = picviz_properties_get(line->props, "color");

	cairo_set_source_rgb(cr,
			picviz_color_extract_r(color),
			picviz_color_extract_g(color),
			picviz_color_extract_b(color));

	line_width = atof(picviz_properties_get(line->props, "penwidth")) * 0.4;
	if (line_width) {
		cairo_set_line_width (cr, atof(picviz_properties_get(line->props, "penwidth")));
	}
	if (!strcmp(parameter, "curves")) {
		/* We draw curves */
		if ( y1 < (image->height/2) ) {
			cairo_curve_to(cr, (double)x1, (double)y1, (double)(x2-((x2-x1)/2)), (double)(image->height/2)-((y1-y2)/2), (double)x2, (double)y2);
		} else {
			cairo_curve_to(cr, (double)x1, (double)y1, (double)(x2-((x2-x1)/2)), (double)(image->height/2)+((y1+y2)/2), (double)x2, (double)y2);
		}
	} else {
		/* We draw straight lines */
		cairo_move_to(cr, (double)x1, (double) y1);
		cairo_line_to(cr, (double)x2, (double) y2);
	}
	cairo_stroke(cr);

	lpflags = LPF_DEFAULTS;
	if (engine.display_minmax) lpflags = LPF_MINMAXONLY;
	picviz_line_draw_text(image, lpflags, axis_id, x1, y1, x2, y2, axisplot1, axisplot2, draw_text);

}

cairo_status_t outstdout(void *closure _U_, unsigned char *data, unsigned int length)
{
	if ( fwrite(data, 1, length, stdout) != length )
		return CAIRO_STATUS_WRITE_ERROR;

	return CAIRO_STATUS_SUCCESS;
}

void init(void)
{
	int i;

	for (i = 0; i <= PICVIZ_MAX_AXES; i++) {
		draw_rawtext[i] = 1; /* Defaults = draw text every line */
	}
}

void output(PicvizImage *image, char *arg)
{
	PicvizAxis *axis;
	PicvizLine *line;
	cairo_surface_t *surface;
	/* cairo_surface_t *worldmap; */
	cairo_surface_t *logo;
	unsigned int counter;

	if (engine.output_file) {
	        picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_PLUGIN, "Output pngcairo image to '%s'", engine.output_file);
	} else {
	        picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_PLUGIN, "Output pngcairo image to stdout");
	}

	init();

	parameter = arg;
	if (!parameter) parameter = "";
	/* worldmap = cairo_image_surface_create_from_png("/home/toady/local/simple-map.png"); */

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			(int)image->width, (int)image->height);

	cr = cairo_create (surface);

	cairo_rectangle (cr, 0, 0, (int)image->width, (int)image->height);
	cairo_set_source_rgba (cr,
			       picviz_color_extract_r(image->bgcolor),
			       picviz_color_extract_g(image->bgcolor),
			       picviz_color_extract_b(image->bgcolor),
			       image->bgalpha);
	cairo_fill (cr);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width (cr, 0.5);

	/* Draw each axis and header titles */
	counter = 0;
	while (image->axesorder[counter]) {
		PicvizAxis *axis = (PicvizAxis *)picviz_axis_get_from_name(image, image->axesorder[counter]);

		/* Draw vertical lines */
		cairo_move_to(cr, (double) picviz_axis_position_get(counter), 
			      (double) image->header_height);
		cairo_line_to(cr, (double) picviz_axis_position_get(counter), 
			      (double)image->height);
		counter++;
	}
	cairo_stroke(cr);

	if (image->header_height) {
		//cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_move_to(cr, (double)0, (double)image->header_height);
		cairo_line_to(cr, (double)image->width, (double)image->header_height);

		counter = 0;
		while (image->axesorder[counter]) {
			PicvizAxis *axis = (PicvizAxis *)picviz_axis_get_from_name(image, image->axesorder[counter]);
			char font_size;

			font_size = image->font_size ? image->font_size : 
				                       image->height / engine.font_factor;

			cairo_text_extents_t extents;
			char *label = picviz_properties_get(axis->props, "label");
			cairo_text_extents(cr, label, &extents);
			draw_text(picviz_axis_position_get(counter) - (extents.width/2), 
				  image->header_height - 5,
				  image->font_color,
				  font_size,
				  label);

			counter++;
		}
		cairo_stroke(cr);
	}

	llist_for_each_entry(line, &image->lines, list) {
		if ( ! line->hidden ) {
			picviz_line_draw(image, line, draw_line);
		}
	}

	/* Input logo file */
	if (image->logo) {
		logo = cairo_image_surface_create_from_png(image->logo);
		cairo_set_source_surface(cr, logo, image->logo_x, image->logo_y);
		cairo_paint(cr);
	}

	if (! engine.output_file ) {
		cairo_surface_write_to_png_stream(surface, (cairo_write_func_t)outstdout, NULL);
	} else {
		cairo_surface_write_to_png(surface, engine.output_file);
	}

	cairo_destroy (cr);
	cairo_surface_destroy (surface);

	picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_PLUGIN, "Done");
}
