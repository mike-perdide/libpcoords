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
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcoords.h>

/**
 * Create a new axisplot structure
 *
 * @return the axisplot structure or NULL on error
 */
PcoordsAxisPlot *pcoords_axisplot_new(void)
{
	PcoordsAxisPlot *axisplot;

	axisplot = malloc(sizeof(*axisplot));
	if ( ! axisplot ) {
		fprintf(stderr, "Cannot initalize axisplot: memory exhausted.\n");
		return NULL;
	}

	axisplot->strval  = NULL;
	axisplot->y       = 0;
	axisplot->axis_id = 0;
	pcoords_properties_new(&axisplot->props);

	return axisplot;
}

void pcoords_axisplot_set_y(PcoordsAxis *axis _U_, PcoordsAxisPlot *axisplot, PcvHeight y)
{
#if 0
 	PcoordsStats *stats;

	stats = (PcoordsStats *)pcoords_hash_get(axis->valcount, (char *)axisplot->strval);
        if ( ! stats ) {
		stats = malloc(sizeof(*stats));
		if ( ! stats) {
			fprintf(stderr, "Error. Cannot allocate axis statistics!\n");
			return;
		}
		stats->counter = 1;
		fprintf(stderr,"SET AXIS VALCOUNT ON ID %llu\n", axis->id);
		pcoords_hash_set(axis->valcount, (char *)axisplot->strval, stats, sizeof(*stats));
        } else {
		stats->counter++;
		pcoords_hash_set(axis->valcount, (char *)axisplot->strval, stats, sizeof(*stats));
	}

#endif
	
	axisplot->y = y;

}

/**
 * Destroy the axisplot
 *
 * @param axisplot the axis plot to destroy
 */
void pcoords_axisplot_destroy(PcoordsAxisPlot *axisplot)
{
	pcoords_properties_destroy(axisplot->props);
	free(axisplot);
}

