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
 *
 * $Id: properties.h 174 2008-09-20 07:57:08Z toady $
 */

#ifndef PICVIZ_PROPERTIES_H
#define PICVIZ_PROPERTIES_H

#include "types.h"

typedef struct pcoords_properties pcoords_properties_t;

int pcoords_properties_set(pcoords_properties_t *props, PcvString key, PcvString value);
PcvString pcoords_properties_get(pcoords_properties_t *props, PcvString key);
int pcoords_properties_new(pcoords_properties_t **props);
void pcoords_properties_destroy(pcoords_properties_t *props);

#endif
