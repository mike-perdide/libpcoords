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

typedef struct picviz_properties picviz_properties_t;

int picviz_properties_set(picviz_properties_t *props, PcvString key, PcvString value);
PcvString picviz_properties_get(picviz_properties_t *props, PcvString key);
int picviz_properties_new(picviz_properties_t **props);
void picviz_properties_destroy(picviz_properties_t *props);

#endif
