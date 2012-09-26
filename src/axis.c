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
 * $Id: axis.c 675 2009-07-15 20:14:02Z toady $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linuxlist.h>

#include <picviz.h>

/**
 * \ingroup PicvizMain
 * @{
 */

/** \file axis.c
 * \brief Create and manipulate axes
 */

static unsigned int id = 0;

/**
 * Creates a new axis
 *
 * @return the new axis or NULL on error
 */
PicvizAxis *picviz_axis_new(void)
{
	PicvizAxis *axis = NULL;

	axis = malloc(sizeof(*axis));
	if ( ! axis ) {
		fprintf(stderr, "Cannot initialize axis: memory exhausted.\n");
		return NULL;
	}
	INIT_LLIST_HEAD(&axis->list);
	axis->id = id++;
	axis->type = DATATYPE_EMPTY;
	/* axis->xpos = 0; */
	axis->ymin = ~0; /* FIXME: that must be auto-adaptative */
	axis->ymax = 0;
	axis->numeric = 0;
	axis->render = PCV_TO_RENDER;

	axis->enum_count = 1;
	picviz_properties_new(&axis->enum_hash);

	picviz_properties_new(&axis->props);
	picviz_properties_set(axis->props, "label", "");
	picviz_properties_set(axis->props, "color", "#000000");

	picviz_hash_new(&axis->valcount);

	return axis;
}

/**
 * Destroys an axis
 *
 * @param axis the axis to destroy
 */
void picviz_axis_destroy(PicvizAxis *axis)
{
	picviz_properties_destroy(axis->props);
	free(axis);
}

/**
 * Get the axis structure from the axis name
 *
 * @param image the Picviz image context
 * @param name the axis name
 *
 * @return the axis structure
 */
PicvizAxis *picviz_axis_get_from_name(PicvizImage *image, PcvString name)
{
	return (PicvizAxis *)picviz_hash_get(image->axes, name);
}


/**
 * Set the datatype of the axis
 *
 * @param axis the axis 
 * @param type the datatype
 */
void picviz_axis_set_type(PicvizAxis *axis, PicvizDataType type)
{
	axis->type = type;
}

/**
 * Set the datatype of the axis from a string
 *
 * @param axis the axis 
 * @param string the datatype name
 */
void picviz_axis_set_type_from_string(PicvizAxis *axis, char *string)
{
	if (!strcmp(string, "timeline")) {
		axis->type = DATATYPE_TIMELINE;
		return;
	}
	if (!strcmp(string, "integer")) {
		axis->type = DATATYPE_INTEGER;
		return;
	}
	if (!strcmp(string, "string")) {
		axis->type = DATATYPE_STRING;
		return;
	}
	if (!strcmp(string, "float")) {
		axis->type = DATATYPE_FLOAT;
		return;
	}
	if (!strcmp(string, "short")) {
		axis->type = DATATYPE_SHORT;
		return;
	}
	if (!strcmp(string, "ipv4")) {
		axis->type = DATATYPE_IPV4;
		return;
	}
	if (!strcmp(string, "char")) {
		axis->type = DATATYPE_CHAR;
		return;
	}
	if (!strcmp(string, "gold")) {
		axis->type = DATATYPE_GOLD;
		return;
	}
	if (!strcmp(string, "years")) {
		axis->type = DATATYPE_YEARS;
		return;
	}
	if (!strcmp(string, "enum")) {
		axis->type = DATATYPE_ENUM;
		return;
	}
	if (!strcmp(string, "ln")) {
		axis->type = DATATYPE_LN;
		return;
	}
	if (!strcmp(string, "port")) {
		axis->type = DATATYPE_PORT;
		return;
	}
	if (!strcmp(string, "b12")) {
		axis->type = DATATYPE_B12;
		return;
	}
	if (!strcmp(string, "usec")) {
		axis->type = DATATYPE_USEC;
		return;
	}

	axis->type = DATATYPE_EMPTY;
}

/**
 * Get the datatype string from the axis
 *
 * @param axis the axis 
 *
 * @return string the datatype name
 */
char *picviz_axis_get_string_from_type(PicvizAxis *axis)
{
	switch(axis->type) {
		case DATATYPE_TIMELINE:
			return "timeline";
		case DATATYPE_INTEGER:
			return "integer";
		case DATATYPE_STRING:
			return "string";
		case DATATYPE_FLOAT:
			return "float";
		case DATATYPE_SHORT:
			return "short";
		case DATATYPE_IPV4:
			return "ipv4";
		case DATATYPE_CHAR:
			return "char";
		case DATATYPE_GOLD:
			return "gold";
		case DATATYPE_YEARS:
			return "years";
		case DATATYPE_ENUM:
			return "enum";
		case DATATYPE_LN:
			return "ln";
		case DATATYPE_PORT:
			return "port";
 	        case DATATYPE_B12:
			return "b12";
	        case DATATYPE_USEC:
		        return "usec";
		default:
			return "*** error ***";
			break;
	}
}

/**
 * Get the position of an axis
 *
 * @param counter the axis id
 *
 * @return X position of the axis
 */
PcvWidth picviz_axis_position_get(unsigned int counter)
{
	PcvWidth xpos;

	if (counter == 0) {
		xpos = engine.initial_axis_x_cursor;
	} else {
		xpos = engine.initial_axis_x_cursor + (counter * engine.axis_default_space);
	}

	return xpos;
}

