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
 *
 * $Id: variable.h 623 2009-05-27 21:13:21Z toady $
 */

#ifndef _VARIABLE_H_
#define _VARIABLE_H_

//#include <pcoords.h>
#include <types.h>

#ifdef __cplusplus
 extern "C" {
#endif

enum variable_type_t {
/* So basic that a simple function call is enough
 * to get the variable function mapping */
  PCV_VARTYPE_BASIC, 
/* We need all the data this variables contains 
 * to be mapped first so that we can then redefine
 * the mappings
 */
  PCV_VARTYPE_NEEDALL,
} variable_type_t;
typedef enum variable_type_t PcvVarType;

typedef PcvHeight (*PcoordsVariableMap)(PcvString value);
struct variable_t {
        struct llist_head list;
        PcvString name; /* debug, integer, string, ...*/
        PcvVarType type;
        PcvHeight maxval; /* <- pcoords_variable_max*/
        PcoordsVariableMap *mapping_func;
} variable_t;
typedef struct variable_t PcoordsVariable;

/* Create a new variable type */
PcoordsVariable *pcoords_variable_new(PcvString name);
void pcoords_variable_destroy(PcoordsVariable *var);


/* The string we use as reference to put our variables on its type.
 * If the string we deal with is bigger than this one, it will then become
 * the reference for the rest of the graph.
 */
#define STRING_TYPE_MAX "The competent programmer is fully aware of the limited size of his own skull. He therefore approaches his task with full humility, and avoids clever tricks like the plague."
/* Same value but calculated using values-mapping (_UNIT_TEST_) */
#define STRING_TYPE_BASIC_MAX_YVAL 16105.00
#define STRING_TYPE_MAX_YVAL ((PcvHeight) ~0)

#define EMPTY_TYPE_MAX_YVAL 1 /* Will make disorder and help folks writting better pcv file */

#define CHAR_TYPE_MAX_YVAL 255
#define B12_TYPE_MAX_YVAL 4096
#define SHORT_TYPE_MAX_YVAL 32767
#define INTEGER_TYPE_MAX_YVAL 65535
#define FLOAT_TYPE_MAX_YVAL INTEGER_TYPE_MAX_YVAL

#define TIMELINE_TYPE_MAX_YVAL 86399 /* 23:59 in seconds */
#define GOLDNUMBER_TYPE_MAX_YVAL 1433
#define IPV4_TYPE_MAX_YVAL 4294967295UL /* -> 11111111 11111111 11111111 11111111 */

#define YEARS_TYPE_MAX_YVAL 2147382000

#define USEC_TYPE_MAX_YVAL 999999

//PcvHeight pcoords_variable_max(datatype_t vartype);
PcvHeight pcoords_variable_max(PcoordsImage *image, PcoordsAxis *axis, int string_algo);

#ifdef __cplusplus
 }
#endif

#endif /* _VARIABLE_H_ */

