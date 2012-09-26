/*
 * Picviz - Parallel coordinates ploter
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
 * $Id: filter.h 481 2009-04-09 21:13:34Z toady $
 */

#ifndef _FILTER_H_
#define _FILTER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "linuxlist.h"
#include "types.h"
#include "pcimage.h"

typedef enum picviz_filter_options_t {
        PF_OPTIONS_NONE,
        PF_OPTIONS_PLOTPERCENT /* We have plot > 90% */
} picviz_filter_options_t;
typedef enum picviz_filter_options_t PicvizFilterOptions;

typedef enum picviz_filter_type_t {
        PF_TYPE_ERROR       = 0x00,
        /* filter data as they are */
        PF_VALUE_FILTER     = 0x01,
        /* rendering engine filter */
        PF_PLOT_FILTER      = 0x02,
        /* filter on color */
        PF_COLOR_FILTER     = 0x04,
        /* what we can filter when there is no relation
         * among axes and should not wait for other lines
         * to be applied first */
        PF_PRE_LINE_FILTER  = 0x08,
        /* most cpu consuming filter, we first add lines
         * to then remove them */
        PF_POST_LINE_FILTER = 0x0F,
        /* Filter a line frequency */
        PF_FREQ_FILTER      = 0xF0,
} picviz_filter_type_t;
typedef enum picviz_filter_type_t PicvizFilterType;

typedef enum picviz_filter_relation_t {
        PF_RELATION_ERROR,
        PF_RELATION_EQUAL,
        PF_RELATION_NOTEQUAL,
        PF_RELATION_GREATER,
        PF_RELATION_LESS,
        PF_RELATION_LESS_OR_EQUAL,
        PF_RELATION_GREATER_OR_EQUAL
} picviz_filter_relation_t;
typedef enum picviz_filter_relation_t PicvizFilterRelation;

typedef struct picviz_filter_criterion {
        PicvizFilterType type;
        PicvizFilterRelation relation;
        PicvizFilterOptions options;
        int axis;

        union {
                PcvString data;
                PcvHeight plot;
                double    numfloat;
        } value;

        struct picviz_filter_criterion *and, *or;
} picviz_filter_criterion_t;


typedef struct picviz_filter {
        picviz_filter_criterion_t *criterion;
} picviz_filter_t;

typedef struct picviz_filter PicvizFilter;

PicvizFilter *picviz_filter_new(void);
picviz_filter_criterion_t *picviz_filter_criterion_new(void);
PicvizFilterType picviz_filter_validate(PcvString string);

picviz_filter_criterion_t *picviz_filter_and_criterion(picviz_filter_criterion_t *c1, picviz_filter_criterion_t *c2);
picviz_filter_criterion_t *picviz_filter_or_criterion(picviz_filter_criterion_t *c1, picviz_filter_criterion_t *c2);


void picviz_filter_set_string(PicvizFilter *filter, char *string);

/* defined in filter/filter.yac.y */
PicvizFilter *picviz_filter_build(char *filter);

int picviz_filter_display(picviz_filter_t *filter, pcimage_t *image, struct axisplot_t **axisplot, int axis_max);
int picviz_filter_renplugin(picviz_filter_t *filter, PicvizImage *image, char *freqstr, void *userdata);

#ifdef __cplusplus
 }
#endif

#endif /* _FILTER_H_ */

