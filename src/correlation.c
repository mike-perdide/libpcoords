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
 * $Id: correlation.c 469 2009-04-02 17:45:12Z toady $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "correlation.h"
#include "debug.h"
#include "common.h" /* min() */

/*
 * This function's code was taken from glib
 */
static unsigned int hash_func(PcvString ptr)
{
	unsigned int hv = *ptr;

	if ( hv )
		for ( ptr += 1; *ptr; ptr++ )
			hv = (hv << 5) - hv + *ptr;

	return hv;
}

static unsigned int get_key_index(const PcvString key)
{
	return (hash_func(key) % CORRELATION_HASH_SIZE);
}

static PcoordsCorHash *elem_search(struct llist_head *list, const PcvString key)
{
	struct llist_head *tmp;
	PcoordsCorHash *elem;

	llist_for_each(tmp, list) {
		elem = llist_entry(tmp, PcoordsCorHash, list);

		if ( strcmp(elem->key, key) == 0 )
			return elem;
	}

	return NULL;
}

int pcoords_correlation_new(PcoordsCorrelation **correlation)
{
	int i;

	*correlation = malloc(sizeof(**correlation));
	if ( ! *correlation ) {
		fprintf(stderr, "*** Out of memory: cannot correlate!\n");
		return -1;
	}

	(*correlation)->hashes = malloc(CORRELATION_HASH_SIZE * sizeof(*(*correlation)->hashes));
	if ( ! (*correlation)->hashes ) {
		free(*correlation);
		pcoords_debug(PCOORDS_DEBUG_CRITICAL, PCOORDS_AREA_CORE, "Cannot allocate correlation hash!");
		return -1;
	}

	for ( i = 0; i < CORRELATION_HASH_SIZE; i++ ) {
		INIT_LLIST_HEAD(&(*correlation)->hashes[i]);
	}

	return 0;
}

PcvCounter pcoords_correlation_append(PcoordsCorrelation *cor, const PcvString key)
{
        unsigned int idx;
        PcoordsCorHash *elem;

        idx = get_key_index(key);

        elem = elem_search(&cor->hashes[idx], key);
        if ( elem ) {
                elem->value += 1;
                return elem->value;
        }

        elem = malloc(sizeof(*elem));
        if ( ! elem )
                return 0;

        elem->key = strdup(key);
        if ( ! elem->key ) {
                free(elem);
                return 0;
        }

        elem->value = 1;

        llist_add_tail(&elem->list, &cor->hashes[idx]);

        return 1;
}


PcvCounter pcoords_correlation_get(PcoordsCorrelation *cor, PcvString key)
{
        PcoordsCorHash *elem;

        elem = elem_search(&cor->hashes[get_key_index(key)], key);
        if ( ! elem )
                return 0;

        return elem->value;
}

void pcoords_correlation_destroy(PcoordsCorrelation *cor)
{
        int i;
        PcoordsCorHash *elem;
        struct llist_head *tmp, *bkp;

        for ( i = 0; i < CORRELATION_HASH_SIZE; i++ ) {
                llist_for_each_safe(tmp, bkp, &cor->hashes[i]) {
                        elem = llist_entry(tmp, PcoordsCorHash, list);

                        llist_del(&elem->list);

                        free(elem->key);
                        free(elem);
                }
        }

        free(cor->hashes);
        free(cor);
}

/*
 * Scales a number between 0 and 1 to a heatline color code
 * 0 = green, 0.5 = yellow, 1 = red
 * @Returns a Pcv string such as "#ff0000"
 */
PcvString pcoords_correlation_heatline_get(double value)
{
	PcvString buf;

	double green = 0;
	double red = 0;

	int offset_red;
	int offset_green;

	if (value > 1) {
		fprintf(stderr, "Cannot correlate '%f' (value > 1)\n", value);
		return NULL;
	}
	if (value < 0) {
		fprintf(stderr, "Cannot correlate '%f' (value < 0)\n", value);
		return NULL;
	}

	/* DindinX rocks! */
	green = min(2-2*value,1);
	red   = min(2*value,1);

	buf = malloc(8); /* "#ff3e00\0" */
	offset_red = (int) (red*255);
	offset_green = (int) (green*255);
	if ((offset_red < 10) && (offset_green >= 10)) {
		snprintf(buf, 8, "#0%X%X00", offset_red, offset_green);
	}
	if ((offset_red >= 10) && (offset_green < 10)) {
		snprintf(buf, 8, "#%X0%X00", offset_red, offset_green);
	}
	if ((offset_red < 10) && (offset_green < 10)) {
		snprintf(buf, 8, "#0%X0%X00", offset_red, offset_green);
	}
	if ((offset_red > 10) && (offset_green > 10)) {
		snprintf(buf, 8, "#%X%X00", offset_red, offset_green);
	}

	if (engine.debug) {
		fprintf(stdout, "We send the color %s; ratio=%f\n", buf, value);
	}
	return buf;
}

int pcoords_correlation_heatline_get_red(double value)
{
	double red = 0;

	if (value > 1) {
		fprintf(stderr, "Cannot correlate a value > 1\n");
		return 0;
	}
	if (value < 0) {
		fprintf(stderr, "Cannot correlate a value < 0\n");
		return 0;
	}

	red   = min(2*value,1);

	return (int) (red*255);
}

int pcoords_correlation_heatline_get_green(double value)
{
	double green = 0;

	if (value > 1) {
		fprintf(stderr, "Cannot correlate a value > 1\n");
		return 0;
	}
	if (value < 0) {
		fprintf(stderr, "Cannot correlate a value < 0\n");
		return 0;
	}

	green = min(2-2*value,1);

	return (int) (green*255);
}


#ifdef _UNIT_TEST_
#include <stdio.h>
int main(void)
{
	char *buf;
	buf = pcoords_correlation_heatline_get(0.123);
	printf("My color=%s\n",buf);
	free(buf);

	return 0;
}
#endif
