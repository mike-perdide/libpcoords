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
#include <stdlib.h>
#include <string.h>

#include <picviz.h>

/**
 * Create a new axisplot structure
 *
 * @return the axisplot structure or NULL on error
 */
PicvizAxisPlot *picviz_axisplot_new(void)
{
	PicvizAxisPlot *axisplot;

	axisplot = malloc(sizeof(*axisplot));
	if ( ! axisplot ) {
		fprintf(stderr, "Cannot initalize axisplot: memory exhausted.\n");
		return NULL;
	}

	axisplot->strval  = NULL;
	axisplot->y       = 0;
	axisplot->axis_id = 0;
	picviz_properties_new(&axisplot->props);

	return axisplot;
}

void picviz_axisplot_set_y(PicvizAxis *axis _U_, PicvizAxisPlot *axisplot, PcvHeight y)
{
#if 0
 	PicvizStats *stats;

	stats = (PicvizStats *)picviz_hash_get(axis->valcount, (char *)axisplot->strval);
        if ( ! stats ) {
		stats = malloc(sizeof(*stats));
		if ( ! stats) {
			fprintf(stderr, "Error. Cannot allocate axis statistics!\n");
			return;
		}
		stats->counter = 1;
		fprintf(stderr,"SET AXIS VALCOUNT ON ID %llu\n", axis->id);
		picviz_hash_set(axis->valcount, (char *)axisplot->strval, stats, sizeof(*stats));
        } else {
		stats->counter++;
		picviz_hash_set(axis->valcount, (char *)axisplot->strval, stats, sizeof(*stats));
	}

#endif
	
	axisplot->y = y;

}

/**
 * Destroy the axisplot
 *
 * @param axisplot the axis plot to destroy
 */
void picviz_axisplot_destroy(PicvizAxisPlot *axisplot)
{
	picviz_properties_destroy(axisplot->props);
	free(axisplot);
}

