/*
 * Picviz - Parallel coordinates ploter
 * Copyright (C) 2009 Sebastien Tricaud <toady@gscore.org>
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
 * $Id: picviz-hash.h 599 2009-05-24 14:12:05Z toady $
 */

#ifndef PICVIZ_HASH_H
#define PICVIZ_HASH_H

#include "types.h"

typedef struct picviz_hash picviz_hash_t;

int picviz_hash_set(picviz_hash_t *hash, const PcvString key, const void *value, size_t size);
PcvString picviz_hash_get(picviz_hash_t *hash, PcvString key);
int picviz_hash_new(picviz_hash_t **hash);
void picviz_hash_destroy(picviz_hash_t *hash);

#endif
