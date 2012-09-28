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
 * $Id: picviz-init.c 659 2009-06-26 21:42:59Z toady $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include <picviz.h>


/*! \mainpage Picviz library
 *
 * \section intro Introduction
 *
 * Picviz library provides API for programmers to use
 * picviz features. The pcv program is the biggest user
 * of the picviz library, you may have a look at it.
 *
 * From the very beginning, Picviz was designed to be embeded
 * by third party software. This is why this library.
 * 
 * The Picviz library allows to access to the parser, the
 * rendering and their plugins, but also to output plugins.
 *
 * Picviz has the following architecture:
 \dot
  digraph picvizarch {
rankdir = LR;
  node [shape=record];
      pgdl [ label="PGDL"];
      parser [ label="Parser"];
      rendering [ label="Rendering"];
      image [ label="Image"];
      
      pgdl -> parser;
      parser -> rendering;
      rendering -> image;

  }
 \enddot
 *
 * \ref PicvizParser
 * 
 * \ref PicvizOutputPlugins
 *
 * \ref PicvizRenderingPlugins
 *
 */


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


