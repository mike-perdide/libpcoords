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
 * $Id: pcoords-pcre.c 469 2009-04-02 17:45:12Z toady $
 */

#include <stdio.h>
#include <string.h>
#include <pcre.h>

#include "types.h"
#include "pcoords-pcre.h"

/*
 * we need to specify the vector length for our pcre_exec call.  we only care
 * about the first vector, which if the match is successful will include the
 * offset to the end of the full pattern match.  If we decide to store other
 * matches, make *SURE* that this is a multiple of 3 as pcre requires it.
 */
#define PICVIZ_PCRE_OVECTOR_SIZE 3

PcoordsBool pcoords_regex_match(char *string, char *regex)
{
	pcre *regexptr;
	pcre_extra *extra;
	const char *errptr;
	int erroffset;
	int retval;
	size_t stringlen;
	int ovector[PICVIZ_PCRE_OVECTOR_SIZE];

	if (!string)
		return BOOL_ERROR;

	stringlen = strlen(string);

	regexptr = pcre_compile(regex, 0, &errptr, &erroffset, NULL);
	if ( ! regexptr ) {
		fprintf(stderr, "Unable to compile regex[offset:%d]: %s.\n", erroffset, errptr);
		return BOOL_ERROR;
	}

	extra = pcre_study(regexptr, 0, &errptr);

	retval = pcre_exec(regexptr, extra, string, stringlen, 0, 0, ovector, PICVIZ_PCRE_OVECTOR_SIZE);
	if ( retval >= 0 ) {
		goto outsuccess;
	}

	goto outfailed;

outfailed:
	pcre_free(regexptr);
	pcre_free(extra);
	return BOOL_FALSE;
outsuccess:
	pcre_free(regexptr);
	pcre_free(extra);
	return BOOL_TRUE;
}

