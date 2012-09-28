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
 * $Id: axis.h 601 2009-05-24 16:21:26Z toady $
 */

#ifndef _PGDL_H_
#define _PGDL_H_
#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif


typedef enum section_t {
        EMPTY,
        HEADER,
        ENGINE,
        AXES,
        DATA
} section_t;

enum pcoords_parser_mode_t {
        PCOORDS_PARSER_MODE_FILE,
	PCOORDS_PARSER_MODE_LINE
} pcoords_parser_mode_t;
typedef enum pcoords_parser_mode_t PcoordsParserMode;

struct pcoords_parser_context_t {
        section_t section_state;
        FILE *yyin;
        PcoordsParserMode mode;
} pcoords_parser_context_t;
typedef struct pcoords_parser_context_t PcoordsParserContext;

FILE *yyin;

static section_t section_state = EMPTY;
struct pcimage_t *image;
struct line_t *line;
char *line_color;
char *line_penwidth;
/* I know this is ugly,
 * I'll get back to it later
 */
#define FILE_MODE 0
#define LINE_MODE 1
/* char FILE_OR_LINE = FILE_MODE; */
char FILE_OR_LINE;

PcoordsImage *pcv_parse(char *filename, char *filterbuf);
PcoordsLine *pcoords_parse_line(char *string);

#ifdef __cplusplus
 }
#endif

#endif /* _PGDL_H_ */

