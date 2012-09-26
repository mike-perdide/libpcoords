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
 * $Id: picviz-init.c 610 2009-05-24 22:10:13Z toady $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include <picviz.h>

/**
 * Initialize the Picviz library
 *
 * @param argc: Address of the argc parameter of your main() function.
 * @param argv: Address of the argv parameter of your main() function.
 */
void picviz_init(int *argc, char **argv)
{
	picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_CORE, "Picviz - (c) 2008-2009 Sebastien Tricaud");
}

