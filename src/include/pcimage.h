/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2008-2009 Sebastien Tricaud <toady@gscore.org>
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
 * $Id: pcimage.h 657 2009-06-26 18:45:52Z toady $
 */

/* core datastructure */

#ifndef _PCIMAGE_H_
#define _PCIMAGE_H_

typedef struct pcimage_t pcimage_t;

#include "linuxlist.h"
#include "engine.h"
#include "types.h"
#include "properties.h"
#include "correlation.h"
#include "picviz-hash.h"

#ifdef __cplusplus
 extern "C" {
#endif

enum is_rendered_t {
        PCV_TO_RENDER,   /* When axis values need to be rendered */
	PCV_RENDER_DONE /* When axis values have been previously rendered */
} is_rendered_t;
typedef enum is_rendered_t PicvizRender;

/**
 * Picviz variable type
 */
enum datatype_t {
	DATATYPE_EMPTY,
	DATATYPE_INTEGER,
	DATATYPE_NUMERIC, /* Where the max value is set as the variable name (4096 axis1 [label="Axis 1"];)*/
	DATATYPE_FLOAT,
	DATATYPE_STRING,
	DATATYPE_TIMELINE,
	DATATYPE_SHORT,
	DATATYPE_IPV4,
	DATATYPE_GOLD,
	DATATYPE_B12,
	DATATYPE_CHAR,
	DATATYPE_YEARS,
	DATATYPE_ENUM,
	DATATYPE_LN,
	DATATYPE_PORT,
	DATATYPE_USEC,
} datatype_t;
typedef enum datatype_t PicvizDataType;

/**
 * Picviz plot structure
 */
struct axisplot_t {
        PcvID axis_id; /* id of the axis we positionate the line to */
        PcvString strval; /* This is the string value of the ysource, to put (optionally) in the graph */
        PcvHeight y;         /* where does the line goes on this axis */
        picviz_properties_t *props;
} axisplot_t;
typedef struct axisplot_t PicvizAxisPlot;

/**
 * Picviz line structure
 */
struct line_t {
        struct llist_head list;
	picviz_hash_t *axesplots;
        PcvID id;
        PcvString layer;
        unsigned char hidden;
        picviz_properties_t *props;
} line_t;
typedef struct line_t PicvizLine;

/**
 * Picviz axis structure
 */
struct axis_t {
        struct llist_head list;
        PcvID id;
	PcvString name;
	PicvizRender render;
        picviz_properties_t *props;
        PicvizDataType type;
        PcvHeight ymin;  /* Where we start (top): Not max line, max of what we see */
        PcvHeight ymax;  /* Where we end (botton): Not min line */
	/* For enumeration type */
	int enum_count;
	picviz_properties_t *enum_hash;
	unsigned long long int numeric;
        picviz_hash_t *valcount; /* Used to count the number of time a given value repeats */
} axis_t;
typedef struct axis_t PicvizAxis;

/**
 * Picviz image structure
 */
struct pcimage_t {
	PcvString axesorder[PICVIZ_MAX_AXES+1];  /* Stores which axis should come when. +1 to have the latest elem to NULL if we have 1024 axes!*/
	PcvWidth  width;
	PcvHeight height;
	PcvHeight header_height;
	PcvString title;
	PcvString bgcolor;
	double    bgalpha;
	PcvString logo;
	PcvWidth  logo_x;
	PcvHeight logo_y;
        PcvString font_color;
        char font_size;
	void  *filter;
	PicvizCorrelation *correlation;

	enum position_t zero_pos; /* where the zero value is on the axes */

	picviz_hash_t *axes;
        picviz_hash_t *hidden_layers; /* if NULL the layer is displayed, hidden otherwise */
        struct llist_head lines;
	PcvCounter lines_max;
};
typedef struct pcimage_t PicvizImage;

#ifdef __cplusplus
 }
#endif

#endif /* _PCIMAGE_H_ */

