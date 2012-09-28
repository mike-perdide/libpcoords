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
 * $Id: line.h 602 2009-05-24 19:13:59Z toady $
 */

#ifndef _LINE_H_
#define _LINE_H_

#include <pcoords.h>

#include "linuxlist.h"

#ifdef __cplusplus
 extern "C" {
#endif

enum line_print_text_flags_t {
  LPF_DEFAULTS,
  LPF_MINMAXONLY,
};
typedef enum line_print_text_flags_t LinePrintTextFlags;

void pcoords_line_free(PcoordsImage *image, PcoordsLine *line);
struct line_t *pcoords_line_init(void);
struct line_t *pcoords_line_new(void);
struct line_t *pcoords_line_get(PcoordsAxis *axis, PcvID id);
//void pcoords_lines_axisplot_append(PcoordsLine *line, PcoordsAxisPlot *axisplot);
void pcoords_line_axis_append(PcoordsLine *line, PcvString axisname, PcoordsAxisPlot *axisplot);
PcvHeight pcoords_line_max_get(pcimage_t *image, struct llist_head *line, PcvID axis_id);
PcvHeight pcoords_line_max_pertype_get(PcoordsImage *image, PcoordsDataType type);
int pcoords_line_draw(PcoordsImage *image, PcoordsLine *line, void (*draw_line_func)(PcoordsImage *image, PcvID axis_id, PcoordsLine *line, PcoordsAxisPlot *axisplot1, PcoordsAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2));
PcoordsLine *pcoords_line_id_get(PcoordsImage *image, PcvID line_id);
void pcoords_line_debug(PcoordsLine *line);
void pcoords_line_axis_foreach(PcoordsImage *image, PcoordsLine *line, void (*each_axis_function)(PcvString key, PcoordsAxisPlot *axisplot));
void pcoords_line_axis_foreach_unique(PcoordsImage *image, PcoordsLine *line, void (*each_axis_function)(PcvString key, PcoordsAxisPlot *axisplot));
unsigned int pcoords_line_all_axisplot_get(PcoordsImage *image, PcoordsLine *line, PcoordsAxisPlot **ap);
void pcoords_line_draw_text(PcoordsImage *image, LinePrintTextFlags flags, PcvID axis_id, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2, PcoordsAxisPlot *axisplot1, PcoordsAxisPlot *axisplot2, void (*draw_text)(double x, double y, char *color, double size, char *text));

#ifdef __cplusplus
 }
#endif

#endif /* _LINE_H_ */

