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
 * $Id: learn.c 599 2009-05-24 14:12:05Z toady $
 */

#include <stdio.h>
#include <string.h>

#include <picviz.h>

void picviz_learn(PicvizImage *image)
{
#if 0
	PicvizLine *line;
	PicvizAxisPlot *axisplot;
	char *string_algo;
	PcvString lowest[PICVIZ_MAX_AXES];
	PcvString highest[PICVIZ_MAX_AXES];
	PcvCounter j;

	if (!engine.learn) return;

	/* Lowest */
	line = picviz_line_id_get(image, 0);
	if ( ! line ) {
		return; /* We cannot learn! */
	}
	llist_for_each_entry(axisplot, &line->axisplot, list) {
		lowest[axisplot->axis_id] = axisplot->strval;
	}

	/* Highest */
	line = picviz_line_id_get(image, (PcvID)image->lines_max-1);
	if ( ! line ) {
		return; /* We cannot learn! */
	}
	llist_for_each_entry(axisplot, &line->axisplot, list) {
		highest[axisplot->axis_id] = strdup(axisplot->strval);
	}

	for (j = 0; j <= image->lines_max; j++) {
		PicvizAxis *axis = (PicvizAxis *)picviz_axis_get(image, j);

		if (axis) {
			if (axis->type == DATATYPE_STRING) {
				if (!strncmp(lowest[axis->id],highest[axis->id], sizeof(int))) {
					/* We can use the basic algo */
					string_algo = picviz_properties_get(axis->props, "algo");
					if ( ! string_algo ) { /* No algo set? we need to learn */
						picviz_properties_set(axis->props, "algo","basic");
					}
				} else {
					/* We dont use the basic algo */
					string_algo = picviz_properties_get(axis->props, "algo");
					if ( ! string_algo ) { /* No algo set? we need to learn */
						picviz_properties_set(axis->props, "algo","nocol");
					}
				}
			}
		}
	}
#endif
}

