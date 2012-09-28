/*
 * Pcoords - Parallel coordinates ploter
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
 * $Id: plugins.h 410 2009-02-24 09:12:38Z toady $
 */

#ifndef _PLUGINS_H_
#define _PLUGINS_H_

#include "pcimage.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define PCOORDS_OUTPUT_API_VERSION "1"

typedef enum plugin_type_t {
        PCOORDS_PLUGIN_UNKNOWN,
        PCOORDS_PLUGIN_OUTPUT,
        PCOORDS_PLUGIN_RENDER,
	PCOORDS_PLUGIN_VARIABLE	
} plugin_type_t;
typedef enum plugin_type_t PcoordsPluginType;


typedef struct pcoords_plugin_t {
        struct llist_head list;

        unsigned int id;
        char *name;
        char *api_version;

        PcoordsPluginType type;

        int (*subscribe)(void);
        void (*unsubscribe)(void);

} pcoords_plugin_t;

void image_to_svg(struct pcimage_t *image);

void pcoords_plugin_load(PcoordsPluginType plugin_type, char *plugin_name, struct pcimage_t *image, PcvString arg);

#ifdef __cplusplus
 }
#endif

#endif /* _PLUGINS_H_ */

