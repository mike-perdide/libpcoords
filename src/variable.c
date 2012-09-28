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
 * $Id: variable.c 623 2009-05-27 21:13:21Z toady $
 */

#include <stdio.h>
#include <linuxlist.h>
#include <pcoords.h>


PicvizVariable *pcoords_variable_new(PcvString name)
{
        PicvizVariable *var = NULL;
  
	var = malloc(sizeof(*var));
	if ( ! var ) {
	        fprintf(stderr, "Cannot initialize var: memory exhausted!\n");
		return NULL;
	}
	INIT_LLIST_HEAD(&var->list);
	var->name = name;
	var->type = PCV_VARTYPE_BASIC;
	var->maxval = 0;
	var->mapping_func = NULL;

	return var;
}

void pcoords_variable_destroy(PicvizVariable *var)
{
        free(var);
}

PcvHeight pcoords_variable_max(PicvizImage *image, PicvizAxis *axis, int string_algo)
{

        switch (axis->type) {
                case DATATYPE_EMPTY:
                        return EMPTY_TYPE_MAX_YVAL;
                case DATATYPE_SHORT:
                        return SHORT_TYPE_MAX_YVAL;
		case DATATYPE_PORT:
		case DATATYPE_INTEGER:
                        return INTEGER_TYPE_MAX_YVAL;
                case DATATYPE_FLOAT:
                        return FLOAT_TYPE_MAX_YVAL;
                case DATATYPE_STRING:
			if (string_algo == 0) {
				return STRING_TYPE_BASIC_MAX_YVAL;
			}
                        return STRING_TYPE_MAX_YVAL;
                case DATATYPE_TIMELINE:
                        return TIMELINE_TYPE_MAX_YVAL;
                case DATATYPE_IPV4:
                        return IPV4_TYPE_MAX_YVAL;
 	        case DATATYPE_USEC:
		        return USEC_TYPE_MAX_YVAL;
                case DATATYPE_CHAR:
                        return CHAR_TYPE_MAX_YVAL;
                case DATATYPE_GOLD:
                        return GOLDNUMBER_TYPE_MAX_YVAL;
                case DATATYPE_YEARS:
                        return YEARS_TYPE_MAX_YVAL;
	        case DATATYPE_B12:
			return B12_TYPE_MAX_YVAL;
                case DATATYPE_ENUM:
                        return image->height;
                case DATATYPE_LN:
                        return image->height;
 	        case DATATYPE_NUMERIC:
		        return axis->numeric;
		default:
			fprintf(stderr, "ERROR! Impossible to map variable!\n");
                        return EMPTY_TYPE_MAX_YVAL;

        }
}
