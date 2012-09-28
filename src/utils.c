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
 * $Id: utils.c 469 2009-04-02 17:45:12Z toady $
 */

/* Utils functions for plugins */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <linuxlist.h>

#include "utils.h"

typedef struct alllines_t {
        struct llist_head list;
        int x1;
        int x2;
        float y1;
        float y2;
} alllines_t;

LLIST_HEAD(lc_list); /* Lines coordinates list */

void pcoords_util_line_append(int x1, float y1, int x2, float y2)
{
        struct alllines_t *alllines;

        alllines = malloc(sizeof(*alllines));

        alllines->x1 = x1;
        alllines->y1 = y1;
        alllines->x2 = x2;
        alllines->y2 = y2;

        llist_add_tail(&alllines->list, &lc_list);
}

/* Add lines x1, x2, y1, y2 to a list, to avoid duplicates */
int pcoords_util_line_exists(int x1, float y1, int x2, float y2)
{
        struct alllines_t *alllines;

        llist_for_each_entry(alllines, &lc_list, list) {
                if ((alllines->x1 == x1) &&
                    (alllines->y1 == y1) &&
                    (alllines->x2 == x2) &&
                    (alllines->y2 == y2)) {
                        return 1;
                    }
        }

        return 0;
}

char *pcoords_string_up(char *str)
{
        int i = 0;
        char *retstr;

        if (!str) return NULL;
        retstr = malloc(strlen(str) + 1);

        while (*str) {
                retstr[i] = toupper(*str++);
                i++;
        }

        retstr[i++] = '\0';

        return retstr;
}

int pcoords_is_string_algo_basic(PcoordsAxis *axis)
{
        char *string_algo;

	if (!axis) {
		fprintf(stderr, "*** Empty axis!\n");
		return 0;
	}

        if (engine.string_algo == 0) return 1;

        string_algo = pcoords_properties_get(axis->props, "algo");
        if (!string_algo) string_algo="";
        if (!strcmp(string_algo, "basic")) {
                //printf("The axis %llu is basic\n",axis->id);
                return 1;
        }

        return 0;
}

#ifdef _UNIT_TEST_
int main(void)
{
        char *str = "this is a string";

        str = pcoords_string_up(str);

        printf("str=[%s]\n", str);
        free(str);
}
#endif

