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
 * $Id: debug.c 607 2009-05-24 21:35:44Z toady $
 */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include <pcoords.h>

void pcoords_debug(int level, int area, const char *format, ...)
{
	char *timestr;
	va_list ap;
	time_t tm;
	FILE *fd = stderr;

	if (engine.quiet) return;

	if (level > engine.debug) return;

	va_start(ap, format);

	if (engine.debug > PICVIZ_DEBUG_CRITICAL) {
		tm = time(NULL);
		timestr = ctime(&tm);
		timestr[strlen(timestr)-1] = '\0';
		fprintf(fd, "%s <%1d.%1d> ", timestr, level, area);
	} else {
		fprintf(fd, "[+] ");
	}
	vfprintf(fd, format, ap);
	va_end(ap);
	fprintf(fd, "\n");
	
	fflush(fd);
}

void pcoords_debug_print_axisplot(PicvizAxisPlot *axisplot)
{
	fprintf(stderr, "axisplot->strval=%s\n", axisplot->strval);
	fprintf(stderr, "axisplot->y=%lld\n", axisplot->y);
	fprintf(stderr, "axisplot->axis_id=%llu\n", axisplot->axis_id);
	fprintf(stderr, "axisplot->props->color=%s\n", pcoords_properties_get(axisplot->props, "color"));
}

