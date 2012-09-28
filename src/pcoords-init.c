/*
 * Pcoords - Parallel coordinates ploter
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
 * $Id: pcoords-init.c 659 2009-06-26 21:42:59Z toady $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include <pcoords.h>


/*! \mainpage Pcoords library
 *
 * \section intro Introduction
 *
 * Pcoords library provides API for programmers to use
 * pcoords features. The pcv program is the biggest user
 * of the pcoords library, you may have a look at it.
 *
 * From the very beginning, Pcoords was designed to be embeded
 * by third party software. This is why this library.
 * 
 * The Pcoords library allows to access to the parser, the
 * rendering and their plugins, but also to output plugins.
 *
 * Pcoords has the following architecture:
 \dot
  digraph pcoordsarch {
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
 * \ref PcoordsParser
 * 
 * \ref PcoordsOutputPlugins
 *
 * \ref PcoordsRenderingPlugins
 *
 */


/**
 * Initialize the Pcoords library
 *
 * @param argc: Address of the argc parameter of your main() function.
 * @param argv: Address of the argv parameter of your main() function.
 */
void pcoords_init(int *argc, char **argv)
{
	pcoords_debug(PCOORDS_DEBUG_NOTICE, PCOORDS_AREA_CORE, "Pcoords - (c) 2008-2009 Sebastien Tricaud");
}


