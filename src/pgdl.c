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
 * $Id$
 */

#include <picviz.h>
#include <string.h>

#include "picviz.h"
#include "pgdl.h"
#include "pgdl-parser.h"

/**
 * @defgroup PicvizParser Picviz Parser
 * Parse a PGDL file
 * @{
 */

static _pgdl_context_init(PicvizParserContext context)
{
        context.section_state = EMPTY;
}

/**
 * Parse a PGDL file into Picviz data structure
 *
 * \param filename the file to parse
 * \param filterbuf the filter to apply
 *
 * \return a PicvizImage object or NULL upon error.
 */
PicvizImage *pcv_parse(char *filename, char *filterbuf)
{
	int ret;
	PicvizParserContext context;

	_pgdl_context_init(context);

	picviz_debug(PICVIZ_DEBUG_NOTICE, PICVIZ_AREA_PARSER, "Parsing");
        FILE_OR_LINE = FILE_MODE;

	line_color = strdup("#000000");
	line_penwidth = strdup(DEFAULT_PENWIDTH);

        image = picviz_image_new();
	if (!image) return NULL;
        if (filterbuf) {
                image->filter = picviz_filter_build(filterbuf);
        }
        yyin = fopen(filename,"r");	
        if ( ! yyin ) {
                picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PARSER, "Cannot open file '%s'", filename);
                return NULL;
        }
	yyrestart(yyin);
	ret  = yyparse ();
	switch (ret) {
	case 1:
		picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PARSER, "Invalid input!\n");
		fclose(yyin);
		return NULL;
		break;
	case 2:
		picviz_debug(PICVIZ_DEBUG_CRITICAL, PICVIZ_AREA_PARSER, "Not enough memory!\n");
		fclose(yyin);
		return NULL;
		break;		
	}

	fflush(yyin);
	fclose(yyin);
	yyin = NULL;

        picviz_render_image(image);

        return image;
}

/**
 * Parse a PGDL data line to be added into Picviz data structure
 *
 * \param string the string containing the line to parse. It must
 * end with a ';' and may look like:
 * "t='12:00', str='mydata';"
 *
 * \return a PicvizLine object or NULL upon error.
 */
PicvizLine *picviz_parse_line(char *string)
{
        void *state;
        int ret;

        FILE_OR_LINE = LINE_MODE;

        section_state = DATA;
        state = yy_scan_string(string);
        ret = yyparse();
        yy_delete_buffer(state);

        return line;
}



/**
 * @}
 */
