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
 * $Id: pcoords-hash.h 599 2009-05-24 14:12:05Z toady $
 */

#ifndef PICVIZ_HASH_H
#define PICVIZ_HASH_H

#include "types.h"

typedef struct pcoords_hash pcoords_hash_t;

int pcoords_hash_set(pcoords_hash_t *hash, const PcvString key, const void *value, size_t size);
PcvString pcoords_hash_get(pcoords_hash_t *hash, PcvString key);
int pcoords_hash_new(pcoords_hash_t **hash);
void pcoords_hash_destroy(pcoords_hash_t *hash);

#endif
