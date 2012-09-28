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
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pcoords.h>

void output(pcimage_t *i, char *arg _U_)
{
	struct axis_t *a;
	struct line_t *l;
	struct axisplot_t *axisplot;
	unsigned int axisnb = 0;
	unsigned int counter;

	llist_for_each_entry(a, &i->axes, list) {
		axisnb++;
	}

	llist_for_each_entry(l, &i->lines, list) {
		if (!l->hidden) {
			counter = 0;
			llist_for_each_entry(axisplot, &l->axisplot, list) {
				counter++;
				if (axisnb == counter)
					printf("%llu",axisplot->y);
				else
					printf("%llu,",axisplot->y);
			}
			printf("\n");
		}
	}

}


