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
 * $Id: properties.c 469 2009-04-02 17:45:12Z toady $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linuxlist.h"
#include "properties.h"
#include "debug.h"

#define DEFAULT_HASH_SIZE 16


struct picviz_properties {
        struct llist_head *lists;
};


typedef struct {
        struct llist_head list;
        PcvString key;
        PcvString value;
} property_elem_t;


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


static property_elem_t *elem_search(struct llist_head *list, const PcvString key)
{
        struct llist_head *tmp;
        property_elem_t *elem;

        llist_for_each(tmp, list) {
                elem = llist_entry(tmp, property_elem_t, list);

                if ( strcmp(elem->key, key) == 0 )
                        return elem;
        }

        return NULL;
}



int picviz_properties_set(picviz_properties_t *props, const PcvString key, const PcvString value)
{
        unsigned int idx;
        property_elem_t *elem;

        idx = get_key_index(key);

        elem = elem_search(&props->lists[idx], key);
        if ( elem ) {
                free(elem->value);

                elem->value = strdup(value);
                if ( ! elem->value )
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

        elem->value = strdup(value);
        if ( ! elem->value ) {
                free(elem->key);
                free(elem);
                return -1;
        }

        llist_add_tail(&elem->list, &props->lists[idx]);

        return 0;
}


PcvString picviz_properties_get(picviz_properties_t *props, PcvString key)
{
        property_elem_t *elem;

        elem = elem_search(&props->lists[get_key_index(key)], key);
        if ( ! elem )
                return NULL;

        return elem->value;
}


int picviz_properties_new(picviz_properties_t **props)
{
        int i;

        *props = malloc(sizeof(**props));
        if ( ! *props ) {
                picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_CORE, "Cannot allocate properties!");
                return -1;
        }

        (*props)->lists = malloc(DEFAULT_HASH_SIZE * sizeof(*(*props)->lists));
        if ( ! (*props)->lists ) {
                free(*props);
                picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_CORE, "Cannot allocate properties hash!");
                return -1;
        }

        for ( i = 0; i < DEFAULT_HASH_SIZE; i++ )
                INIT_LLIST_HEAD(&(*props)->lists[i]);

        return 0;
}


void picviz_properties_destroy(picviz_properties_t *props)
{
        int i;
        property_elem_t *elem;
        struct llist_head *tmp, *bkp;

        for ( i = 0; i < DEFAULT_HASH_SIZE; i++ ) {
                llist_for_each_safe(tmp, bkp, &props->lists[i]) {
                        elem = llist_entry(tmp, property_elem_t, list);

                        llist_del(&elem->list);

                        free(elem->key);
                        free(elem->value);
                        free(elem);
                }
        }

        free(props->lists);
        free(props);
}
