/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2008-2009 Sebastien Tricaud <sebastien@honeynet.org>
 * Copyright (C) 2008 Yoann Vandoorselaere <yoann@prelude-ids.org>
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
 * $Id: picviz-hash.c 600 2009-05-24 16:04:57Z toady $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linuxlist.h"
#include "picviz-hash.h"
#include "debug.h"

#define DEFAULT_HASH_SIZE 16

struct picviz_hash {
        struct llist_head *lists;
};


typedef struct {
        struct llist_head list;
        PcvString key;
        void *value;
} hash_elem_t;


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
        return (hash_func(key) % DEFAULT_HASH_SIZE);
}


static hash_elem_t *elem_search(struct llist_head *list, const PcvString key)
{
        struct llist_head *tmp;
        hash_elem_t *elem;

        llist_for_each(tmp, list) {
                elem = llist_entry(tmp, hash_elem_t, list);

                if ( strcmp(elem->key, key) == 0 )
                        return elem;
        }

        return NULL;
}



int picviz_hash_set(picviz_hash_t *hash, const PcvString key, const void *value, size_t size)
{
        unsigned int idx;
        hash_elem_t *elem;

	if (!hash) {
		fprintf(stderr, "No hash to search for the key '%s'\n", key);
		return -1;
	}
        idx = get_key_index(key);

        elem = elem_search(&hash->lists[idx], key);
        if ( elem ) {
                free(elem->value);

		if ( ! memmove(elem->value, value, size) ) 
                        return -1;

                return 0;
        }

        elem = malloc(sizeof(*elem));
        if ( ! elem )
                return -1;

	elem->key = strdup(key);
	if ( ! elem->key ) {
   	        free(elem);
	        return -1;
	}

	elem->value = malloc(size);
	if ( ! memmove(elem->value, value, size) ) {
                free(elem->key);
                free(elem);
                return -1;
        }
	
        llist_add_tail(&elem->list, &hash->lists[idx]);

        return 0;
}


PcvString picviz_hash_get(picviz_hash_t *hash, PcvString key)
{
        hash_elem_t *elem;

	if ( ! hash) {
		fprintf(stderr, "No hash to search for the key '%s'\n", key);
		return NULL;
	}

        elem = elem_search(&hash->lists[get_key_index(key)], key);
        if ( ! elem )
                return NULL;

        return elem->value;
}


int picviz_hash_new(picviz_hash_t **hash)
{
        int i;

        *hash = malloc(sizeof(**hash));
        if ( ! *hash ) {
                picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_CORE, "Cannot allocate hash!");
                return -1;
        }

        (*hash)->lists = malloc(DEFAULT_HASH_SIZE * sizeof(*(*hash)->lists));
        if ( ! (*hash)->lists ) {
                free(*hash);
                picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_CORE, "Cannot allocate hash!");
                return -1;
        }

        for ( i = 0; i < DEFAULT_HASH_SIZE; i++ )
                INIT_LLIST_HEAD(&(*hash)->lists[i]);

        return 0;
}


void picviz_hash_destroy(picviz_hash_t *hash)
{
        int i;
        hash_elem_t *elem;
        struct llist_head *tmp, *bkp;

        for ( i = 0; i < DEFAULT_HASH_SIZE; i++ ) {
                llist_for_each_safe(tmp, bkp, &hash->lists[i]) {
                        elem = llist_entry(tmp, hash_elem_t, list);

                        llist_del(&elem->list);

                        free(elem->key);
                        free(elem->value);
                        free(elem);
                }
        }

        free(hash->lists);
        free(hash);
}

#ifdef _UNIT_TEST_

typedef struct foobar_t {
	int myint;
	char *mystr;
} foobar_t;

int main(void)
{
	picviz_hash_t *ph;
	struct foobar_t *fooset;
	struct foobar_t *fooget;
	char *getval;
	int *intval;
	int retval;
	int *myi;

	retval = picviz_hash_new(&ph);
	printf("picviz_hash_new retval = %d\n", retval);
	picviz_hash_set(ph, "foo", (char *)"bar", 4);
	picviz_hash_set(ph, "bar", (char *)"foo", 4);
	getval = picviz_hash_get(ph, "foo");
	printf("value 'foo' stores '%s'\n", getval);
	getval = picviz_hash_get(ph, "bar");
	printf("value 'bar' stores '%s'\n", getval);

	/* myi = (int *)19; */
	/* picviz_hash_set(ph, "myint", &myi, sizeof(*myi)); */
	/* intval = (int *)picviz_hash_get(ph, "myint"); */
	/* printf("value 'myint' stores '%d'\n", *intval); */

	fooset = malloc(sizeof(*fooset));
	fooset->myint = 42;
	fooset->mystr = "blahblah";
	picviz_hash_set(ph, "mystruct", fooset, sizeof(*fooset));
	fooget = malloc(sizeof(*fooget));
	fooget = (struct foobar_t *)picviz_hash_get(ph, "mystruct");
	printf("Fooget int='%d', str='%s'\n", fooget->myint, fooget->mystr);
	free(fooset);
	free(fooget);
	picviz_hash_destroy(ph);
	
	return 0;
}
#endif
