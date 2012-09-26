/*
 * Picviz - Parallel coordinates ploter
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
 * $Id: plugins.c 657 2009-06-26 18:45:52Z toady $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <limits.h> /* PATH_MAX */

#include <math.h>

#include <debug.h>
#include <linuxlist.h>
#include <plugins.h>

static char *lastplugin_name;

static void *picviz_plugin_open(char *plugin_name)
{
	char *plugin_path;
	char plugin_full[PATH_MAX];
	void *dlh;
	size_t pathlen = 0;
	size_t namelen = 0;

	plugin_path = getenv("PICVIZ_PLUGINS_PATH");
	pathlen = plugin_path ? strlen(plugin_path) : strlen(PLUGIN_PATH);
	namelen = strlen(plugin_name);
	if ( (pathlen + namelen + 1) > PATH_MAX ) {
		fprintf(stderr, "Plugin path and/or name too long (%s/%s)\n", plugin_path, plugin_name);
		exit(EXIT_FAILURE);
	}
	if (!plugin_path) /* no env variable, we let ld do its job */
		snprintf(plugin_full, sizeof(plugin_full), "%s/%s", PLUGIN_PATH, plugin_name);
	else
		snprintf(plugin_full, sizeof(plugin_full), "%s/%s", plugin_path, plugin_name);

	dlh = dlopen(plugin_full, RTLD_LAZY);
	if ( ! dlh ) {
		fprintf(stderr, "Cannot open plugin: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	lastplugin_name = plugin_full;

	return dlh;
}

void picviz_plugin_load(PicvizPluginType plugin_type, char *plugin_name, PicvizImage *image, PcvString arg)
{

	void *dlh;
	void (*func)(PicvizImage *, PcvString);


	switch(plugin_type) {
		case PICVIZ_PLUGIN_UNKNOWN:
			fprintf(stderr, "Cannot load unknown plugins!\n");
			return;
		case PICVIZ_PLUGIN_OUTPUT:
/**
 * @defgroup PicvizOutputPlugins Picviz Output Plugins
 * Output Plugins provide an API to get read-only data from the
 * rendering engine.
 * @{
 */

			dlh = picviz_plugin_open(plugin_name);
			*(void **)(&func) = dlsym(dlh, "output");
			if ( ! func ) {
				fprintf(stderr, "Symbol output not found in '%s'\n", lastplugin_name);
				exit(EXIT_FAILURE);
			}

			(*func)(image, arg);
/**
 * @}
 */
			break;
		case PICVIZ_PLUGIN_RENDER:
/**
 * @defgroup PicvizRenderingPlugins Picviz Rendering Plugins
 * Rendering Plugins provide an API to get read and write
 * access to the data to drive the rendering engine
 * @{
 */
			dlh = picviz_plugin_open(plugin_name);
			*(void **)(&func) = dlsym(dlh, "render");
			if ( ! func ) {
				fprintf(stderr, "Symbol render not found in '%s'\n", lastplugin_name);
				exit(EXIT_FAILURE);
			}

			(*func)(image, arg);
/**
 * @}
 */
			break;
		default:
			/* Huh? Shouldn't be there*/
			fprintf(stderr, "*** SBT: There is no such plugin type!\n");
			return;

	} /* switch(plugin_type) */

	dlerror();    /* Clear any existing error */

	dlclose(dlh);

}

#if 0
/* Every plugin call this function to register themselves */
void picviz_plugin_register(struct picviz_plugin_t *pp)
{

        if (strcmp(pp->api_version, PICVIZ_OUTPUT_API_VERSION)) {
                picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PLUGIN,
                             "Incompatible version '%s' for plugin. Needed '%s'",
                             pp->api_version, PICVIZ_OUTPUT_API_VERSION);
        }
        if (picviz_plugin_find(pp->name)) {
                picviz_debug(PICVIZ_DEBUG_WARNING, PICVIZ_AREA_PLUGIN,
                             "Plugin '%s' already registered", pp->name);
        } else {
                picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_PLUGIN,
                             "Registering plugin '%s'", pp->name);
                //llist_add(&pp->list, &picviz_plugins);
        }

}
#endif
