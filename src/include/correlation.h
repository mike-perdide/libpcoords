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
 * $Id: correlation.h 286 2008-10-27 08:46:05Z toady $
 */

#ifndef _CORRELATION_H_
#define _CORRELATION_H_

#include "linuxlist.h"
#include "types.h"
//#include <pcoords.h>

#define CORRELATION_HASH_SIZE 16

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum correlation_type_t {
	PCOORDS_COR_NONE,
	PCOORDS_COR_LINE, /* Same line */
	PCOORDS_COR_PLOT  /* Plot shared on a given axis */
} correlation_type_t;
typedef enum correlation_type_t PcoordsCorType;

typedef struct correlation_hash_t {
	struct llist_head list;
	PcoordsCorType type;
	PcvString key; /* "AXISNB:LINE_Y1,LINE_Y2" or "AXISNB:PLOTX" */
	PcvCounter value;
} correlation_hash_t;
typedef struct correlation_hash_t PcoordsCorHash;

typedef struct correlation_t {
	struct llist_head *hashes;
} correlation_t;
typedef struct correlation_t PcoordsCorrelation;

typedef enum heatline_mode_t {
	PER_TWO_AXES, /* Default */
	VIRUS, /* Lines mapped according to the highest frequence in on of two axes and the full line is colored accordingly */
	FROM_POINT, /* If it has the same origin, we look for the frequence */
	PER_FULL_LINE /* Compare line per line instead of between two axes */
} heatline_mode_t;
typedef enum heatline_mode_t PcoordsHLMode;

int pcoords_correlation_new(PcoordsCorrelation **correlation);
PcvCounter pcoords_correlation_append(PcoordsCorrelation *cor, const PcvString key);
PcvCounter pcoords_correlation_get(PcoordsCorrelation *cor, PcvString key);
PcvString pcoords_correlation_heatline_get(double value);
int pcoords_correlation_heatline_get_red(double value);
int pcoords_correlation_heatline_get_green(double value);
void pcoords_correlation_destroy(PcoordsCorrelation *cor);


#ifdef __cplusplus
 }
#endif

#endif /* _CORRELATION_H_ */
