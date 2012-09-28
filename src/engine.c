/*
 * Pcoords - Parallel coordinates ploter
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
 * $Id: engine.c 675 2009-07-15 20:14:02Z toady $
 */

#include <stdio.h>

#include <pcoords.h>

void pcoords_engine_init(void)
{
	engine.real_time = 0; /* Do not listen to a socket by default */
	engine.axis_default_thickness = 1;
	engine.axis_default_space = 100;
	/* engine.axis_x_cursor = engine.axis_default_space / 2; */
	engine.initial_axis_x_cursor = engine.axis_default_space / 2;
	/* engine.axis_x_cursor = 100; */
	engine.__axis_label_exists = 0;
	engine.zero_position = BOTTOM;
	engine.display_raw_data = 0;
	/* Relative mode should be default latter, but too dangerous now */
	engine.relative = 0;
	engine.string_algo = 1;
	engine.use_pcre = 0;
	engine.debug = PICVIZ_DEBUG_CRITICAL; /* Shows notice and critical stuff */
	engine.draw_heatline = 0;
	engine.heatline_algo = 0;
	engine.learn = 1;
	engine.image_height = DEFAULT_IMAGE_HEIGHT;
	engine.image_width = DEFAULT_IMAGE_WIDTH;
	engine.font_factor = 50;
	engine.draw_text = 1; /* default: draw text every line */
	engine.output_file = NULL; /* output is stdout */
	engine.pid_file = PICVIZ_DATADIR "/var/run/pcoords.pid";
	engine.display_minmax = 0;
	engine.quiet = 0; /* default = verbose a little */
}
