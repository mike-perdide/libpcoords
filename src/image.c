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
 * $Id: image.c 657 2009-06-26 18:45:52Z toady $
 */

#include <stdio.h>
#include <stdlib.h>

#include <pcoords.h>

/**
 * \ingroup PcoordsMain
 * @{
 */

/** \file image.c
 * \brief Create and manipulate a parallel coordinates image
 */

struct engine_t engine;

/**
 * Creates a new parallel coordinates empty image
 *
 * @return the new pcoords image or NULL on error
 */
PcoordsImage *pcoords_image_new(void)
{
	PcoordsImage *pcimage = NULL;
	int i;

	pcimage = malloc(sizeof(*pcimage));
	if ( ! pcimage ) {
		fprintf(stderr, "Cannot create image: memory exhausted.\n");
		return NULL;
	}
	for (i=0;i<=PCOORDS_MAX_AXES+1;i++) {
		pcimage->axesorder[i] = NULL;
	}
	pcimage->height        = engine.image_height;
	//pcimage->header_height = DEFAULT_IMAGE_HEADER_HEIGHT;
	pcimage->header_height = pcimage->height / (engine.font_factor+1) + 5;
	pcimage->width         = engine.image_width;
	pcimage->filter        = NULL;
	pcimage->logo          = NULL;
	pcimage->logo_x        = 0;
	pcimage->logo_y        = 0;
	pcimage->zero_pos      = TOP;
	pcimage->title         = "";
	pcimage->bgcolor       = "#FFFFFF"; /* White background default */
	pcimage->bgalpha       = 1; /* Opaque */
	pcimage->lines_max     = 0;
	pcimage->font_color    = "#000000"; /* Black */
	pcimage->font_size     = 0; /* Used only if forced by the user */

	pcoords_hash_new(&pcimage->axes);
	pcoords_hash_new(&pcimage->hidden_layers);

	INIT_LLIST_HEAD(&pcimage->lines);

	pcoords_correlation_new(&pcimage->correlation);

	return pcimage;
}

/**
 * Detroys the image
 *
 * @param image the image to destroy
 */
void pcoords_image_destroy(PcoordsImage *image)
{
	/* PcoordsAxis *axe, *axebkp; */
	struct line_t *line, *linebkp;
	/* unsigned int counter = 0; */

/* 	while (image->axesorder[counter]) { */
/* 	        PcoordsAxis *axis = (PcoordsAxis *)pcoords_axis_get_from_name(image, image->axesorder[counter]); */
/* 		fprintf(stderr,  "ymin=%llu;ymax=%llu\n",axis->ymin, axis->ymax); */
/* /\* 	        if (axis) free(axis); *\/ */
/* 		counter++; */
/* 	} */

	/* llist_for_each_entry_safe(axe, axebkp, &image->axes, list) */
	/* 	pcoords_axis_destroy(axe); */

	llist_for_each_entry_safe(line, linebkp, &image->lines, list) {
		pcoords_line_free(image, line);
	}

	pcoords_correlation_destroy(image->correlation);

	free(image->logo);

	free(image);
}

/**
 * Increases image width
 *
 * @param image the image to update
 * @param expand width value to increase
 */
void pcoords_image_width_increase(PcoordsImage *image, unsigned int expand)
{
        image->width += expand;
}


/**
 * Append an axis to the image
 *
 * @param image the image to update
 * @param axis The PcoordsAxis to append
 */
void pcoords_image_axis_append(PcoordsImage *image, PcoordsAxis *axis)
{
	pcoords_hash_set(image->axes, axis->name, axis, sizeof(*axis));
}

/**
 * Append a line to the image
 *
 * @param image the image to update
 * @param line The PcoordsLine to append
 */
void pcoords_image_line_append(PcoordsImage *image, PcoordsLine *line)
{
	if (line) {
		llist_add_tail(&line->list, &image->lines);
		image->lines_max++;
	}
}

static void pcoords_image_debug_line(PcoordsImage *image, PcvID axis_id, PcoordsLine *line, PcoordsAxisPlot *axisplot1, PcoordsAxisPlot *axisplot2, PcvWidth x1, PcvHeight y1, PcvWidth x2, PcvHeight y2)
{
  printf("line: %s:%llu, %s:%llu\n", axisplot1->strval, y1, axisplot2->strval, y2);
}

void pcoords_image_debug_printall(PcoordsImage *i)
{
        PcoordsAxis *a;
        struct line_t *l;
        struct axisplot_t *axisplot;
	int counter = 0;

        printf("i->width=%d\n", i->width);
        printf("i->height=%lld\n", i->height);
        printf("i->header_height=%lld\n", i->header_height);
        printf("i->zero_pos=%d\n", i->zero_pos);
        printf("i->bgcolor=%s\n", i->bgcolor);
	while (i->axesorder[counter]) {
		PcoordsAxis *a = pcoords_hash_get(i->axes, i->axesorder[counter]);
                printf("    a->id=%llu\n", a->id);
                printf("    a->label=%s\n", pcoords_properties_get(a->props, "label")); 
                printf("    a->type=%d\n", a->type); 
                printf("\n"); 
		counter++;
	}
        llist_for_each_entry(l, &i->lines, list) {
                printf("l->id=%llu\n", l->id);
                printf("l->layer=%s\n", l->layer);
                printf("l->props->color=%s\n", pcoords_properties_get(l->props, "color"));
		if ( ! l->hidden ) {
		  pcoords_line_draw(i, l, pcoords_image_debug_line);
		}
		
                /* llist_for_each_entry(axisplot, &l->axisplot, list) { */
                /*         printf("    axisplot->strval=%s\n", axisplot->strval); */
                /*         printf("    axisplot->y=%lld\n", axisplot->y); */
                /*         printf("    axisplot->axis_id=%llu\n", axisplot->axis_id); */
                /* } */
        }

	printf("axis order: ");
	counter = 0;
	while (i->axesorder[counter]) {
		printf("%s ", i->axesorder[counter]);
		counter++;
	}
	printf("\n");

}

#ifdef _UNIT_TEST_
int main(void)
{
        PcoordsImage *image;
        PcoordsAxis *axis;
        struct line_t *line;
        int i = 0;
        float value;

        pcoords_init();
        image = pcoords_image_new();
        axis = pcoords_axis_init();
        pcoords_image_axis_append(image, axis);
        pcoords_axis_set_type_from_string(axis, "string");
        value = pcoords_line_value_get_from_string(axis, NULL, "foo");
        printf("value=%f\n", value);
        pcoords_axis_prop_set_label(axis, "foo");
        line = pcoords_line_init();
        pcoords_axis_line_append(axis, line);
        axis = pcoords_axis_init();
        pcoords_image_axis_append(image, axis);
        pcoords_axis_set_type_from_string(axis, "integer");
        pcoords_axis_prop_set_label(axis, "bar");
        line = pcoords_line_init();
        pcoords_axis_line_append(axis, line);
        line = pcoords_line_init();
        pcoords_axis_line_append(axis, line);

        pcoords_image_debug_printall(image);

}
#endif

/**
 * @}
 */

