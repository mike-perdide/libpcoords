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
 *
 * $Id: engine.h 610 2009-05-24 22:10:13Z toady $
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#ifdef __cplusplus
 extern "C" {
#endif

enum position_t {
        TOP,
        MIDDLE,
        BOTTOM
} position_t;

/* This is actually a mix of *rendering* and *engine*,
 * which should be sorted out at some point
 */
typedef struct engine_t {
	/* Private members: not recommended to change this at all! */
	char __axis_label_exists; /* Defines wether we have labels in our axis or not */
	char __header_title_exists; /* Defines wether we have title in our graph */

	unsigned int axis_default_thickness;
	unsigned int initial_axis_x_cursor;
	unsigned int axis_x_cursor; /* We move where the X pos of the axis must be put everytime we add one */
	unsigned int axis_default_space;
	enum position_t zero_position;
	int display_raw_data;
	int relative;
	unsigned int string_algo; /* 0 = basic, 1 = better */
	unsigned char use_pcre;
	unsigned char debug;
	unsigned char draw_heatline;
	unsigned char heatline_algo;
	unsigned char learn; /* Read random lines to see which alago can be applied */
	unsigned long long image_height;
	unsigned int image_width;
	unsigned int font_factor;
	unsigned int draw_text;
	char real_time;
	char *output_file;
	char *pid_file;
        unsigned char display_minmax;
	char quiet;
} engine_t;


extern struct engine_t engine;

void picviz_engine_init(void);

#ifdef __cplusplus
 }
#endif

#endif /* _ENGINE_H_ */
