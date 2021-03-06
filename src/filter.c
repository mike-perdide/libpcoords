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
 * $Id: filter.c 777 2009-10-01 20:30:07Z toady $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcoords.h>


static int test_filter_criterion(PcoordsImage *image, pcoords_filter_criterion_t *filter, PcoordsAxisPlot *axisplot)
{
        int ret = 0;
        PcvHeight compare_val;

        if (filter->type == PF_PLOT_FILTER) {
                compare_val = (filter->options == PF_OPTIONS_PLOTPERCENT) ? axisplot->y * 100 / image->height : axisplot->y;

                switch(filter->relation) {
                        case PF_RELATION_ERROR:
                                fprintf(stderr, "ERROR: Filter relation type error!\n");
                                break;

                        case PF_RELATION_EQUAL:
                                if ( filter->value.plot == compare_val)
                                        ret = 1;
                                break;
                        case PF_RELATION_NOTEQUAL:
                                if ( filter->value.plot != compare_val)
                                        ret = 1;
                                break;
                        case PF_RELATION_GREATER:
                                if (filter->value.plot > compare_val)
                                        ret = 1;
                                break;
                        case PF_RELATION_LESS:
                                if (filter->value.plot < compare_val)
                                        ret = 1;
                                break;
                        case PF_RELATION_LESS_OR_EQUAL:
                                if (filter->value.plot <= compare_val)
                                        ret = 1;
                                break;
                        case PF_RELATION_GREATER_OR_EQUAL:
                                if (filter->value.plot >= compare_val)
                                        ret = 1;
                                break;
                        default:
                                fprintf(stderr, "ERROR: Filter relation!\n");
                                break;
                }
        }
        if (filter->type == PF_VALUE_FILTER) {
	        ret = 1;
                switch(filter->relation) {
                        case PF_RELATION_ERROR:
                                fprintf(stderr, "ERROR: Filter relation!\n");
                                break;

                        case PF_RELATION_EQUAL:
				if (engine.use_pcre) {
					if (pcoords_regex_match(axisplot->strval, filter->value.data))
						ret = 0;
				} else {
					if ( !strcmp(filter->value.data, axisplot->strval) )
						ret = 0;
				}
                                break;
                        case PF_RELATION_NOTEQUAL:
				if (engine.use_pcre) {
					if (!pcoords_regex_match(axisplot->strval, filter->value.data))
						ret = 0;
				} else {
					if ( strcmp(filter->value.data, axisplot->strval) )
						ret = 0;
				}
                                break;

                        default:
                                fprintf(stderr, "ERROR: Filter relation!\n");
                                break;
                }
        }

        return ret;
}



static int filter_display_match(PcoordsImage *image, pcoords_filter_criterion_t *criterion, PcoordsAxisPlot **axisplot, int axis_max)
{
        int ret;

        if ( criterion->axis >= axis_max || ! axisplot[criterion->axis] ) {
                fprintf(stderr, "ERROR: axis '%d' does not exist!.\n", criterion->axis);
                return -1;
        }

        ret = test_filter_criterion(image, criterion, axisplot[criterion->axis]);
        if ( ret < 0 )
                return ret;

        if ( ret == 0 && criterion->and )
                ret = filter_display_match(image, criterion->and, axisplot, axis_max);

        if ( ret == 1 && criterion->or )
                ret = filter_display_match(image, criterion->or, axisplot, axis_max);

        if ( ret < 0 )
                return ret;

        return ret;
}



int pcoords_filter_display(pcoords_filter_t *filter, PcoordsImage *image, PcoordsAxisPlot **axisplot, int axis_max)
{
        int ret;

        ret = filter_display_match(image, filter->criterion, axisplot, axis_max);
        if ( ret < 0 )
                return ret;

	return ret;
}

static int test_filter_renplugin_criterion(PcoordsImage *image _U_, pcoords_filter_criterion_t *filter, char *freqstr)
{
        int ret = 0;
	double freq;

	if (!freqstr) return 0;

        if (filter->type == PF_FREQ_FILTER) {
  	        freq = strtod(freqstr, (char **)NULL);

                switch(filter->relation) {
                        case PF_RELATION_ERROR:
                                fprintf(stderr, "ERROR: Filter relation!\n");
                                break;

                        case PF_RELATION_EQUAL:
                                if ( filter->value.numfloat == freq)
                                        ret = 1;
                                break;
                        case PF_RELATION_NOTEQUAL:
                                if ( filter->value.numfloat != freq)
                                        ret = 1;
                                break;
                        case PF_RELATION_GREATER:
                                if (filter->value.numfloat > freq)
                                        ret = 1;
                                break;
                        case PF_RELATION_LESS:
                                if (filter->value.numfloat < freq)
                                        ret = 1;
                                break;
                        case PF_RELATION_LESS_OR_EQUAL:
                                if (filter->value.numfloat <= freq)
                                        ret = 1;
                                break;
                        case PF_RELATION_GREATER_OR_EQUAL:
                                if (filter->value.numfloat >= freq)
                                        ret = 1;
                                break;
                        default:
                                fprintf(stderr, "ERROR: Filter relation!\n");
                                break;
                }
        }


        return ret;
}


static int filter_renplugin_match(PcoordsImage *image, pcoords_filter_criterion_t *criterion, char *freqstr)
{
        return test_filter_renplugin_criterion(image, criterion, freqstr);
}

int pcoords_filter_renplugin(pcoords_filter_t *filter, PcoordsImage *image, char *freqstr, void *userdata)
{
        int ret;
	PcoordsLine *line = (PcoordsLine *)userdata;

        if (filter->criterion->type == PF_FREQ_FILTER) {
	  if (line->hidden) return 1;
	  ret = filter_renplugin_match(image, filter->criterion, freqstr);
	  if ( ret < 0 )
	    return ret;

/* 	fprintf(stderr, "freq=%s,ret=%d\n", freqstr, ret); */

	  return ret;
	} else {
	  return line->hidden; /* We cannot filter. It has been done already so we give back the previously taken decision */
	}
}


PcoordsFilter *pcoords_filter_new(void)
{
        PcoordsFilter *filter;

        filter = malloc(sizeof(PcoordsFilter));
        if (!filter) {
                fprintf(stderr,"Cannot allocate a new filter\n");
                return NULL;
        }

        return filter;
}


pcoords_filter_criterion_t *pcoords_filter_criterion_new(void)
{
        pcoords_filter_criterion_t *c;

        c = malloc(sizeof(*c));
        if ( ! c ) {
                fprintf(stderr,"Cannot allocate a new criterion\n");
                return NULL;
        }

        /* Defaults: safer not to disturb the language parser */
        c->type     = PF_POST_LINE_FILTER;
        c->relation = PF_RELATION_ERROR;
        c->options  = PF_OPTIONS_NONE;
        c->and = c->or = NULL;

        return c;
}


pcoords_filter_criterion_t *pcoords_filter_criterion_clone(pcoords_filter_criterion_t *src)
{
        pcoords_filter_criterion_t *dst;

        dst = pcoords_filter_criterion_new();
        if ( ! dst )
                return NULL;

        memcpy(dst, src, sizeof(*dst));
        if ( src->and )
                dst->and = pcoords_filter_criterion_clone(src->and);

        if ( src->or )
                dst->or = pcoords_filter_criterion_clone(src->or);

        return dst;
}

pcoords_filter_criterion_t *pcoords_filter_and_criterion(pcoords_filter_criterion_t *c1, pcoords_filter_criterion_t *c2)
{
        pcoords_filter_criterion_t *cs = c1, *last, *new;

        while ( c1 ) {
                last = c1;

                if ( c1->or ) {
                        new = pcoords_filter_criterion_clone(c2);
                        if (! new )
                                return NULL;

                        pcoords_filter_and_criterion(c1->or, new);
                }

                c1 = c1->and;
        }

        last->and = c2;
        return cs;
}


pcoords_filter_criterion_t *pcoords_filter_or_criterion(pcoords_filter_criterion_t *c1, pcoords_filter_criterion_t *c2)
{
        while ( c1->or )
                c1 = c1->or;

        c1->or = c2;

        return c1;
}



#ifdef _UNIT_TEST_
int main(void)
{
        PcoordsFilter *filter;

        filter = pcoords_filter_new();
        pcoords_filter_set_string(filter, "show only plotmin 5 on axes");

}
#endif

