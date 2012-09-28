#include <stdio.h>
#include <pcoords.h>

int main(void)
{
        struct pcimage_t *image;
        struct axis_t *axis;
        struct line_t *line;
        struct axisplot_t *axisplot;

        printf("Test3: create lines\n");
        printf("===================\n");
	pcoords_engine_init();
        pcoords_init(NULL, NULL);
        image = pcoords_image_new();
        axis = pcoords_axis_new();
/*         pcoords_image_axis_append(image, axis); */
/*         line = pcoords_line_new(); */
/*         axisplot = pcoords_axisplot_new(); */
/*         axisplot->y = 42; */
/*         axisplot->axis_id = 0; */
/*         pcoords_line_axisplot_append(line, axisplot); */
/*         axisplot = pcoords_axisplot_new(); */
/*         axisplot->y = 12.43; */
/*         axisplot->axis_id = 1; */
/*         pcoords_line_axisplot_append(line, axisplot); */
/*         pcoords_image_line_append(image, line); */

        pcoords_image_debug_printall(image);
        pcoords_image_destroy(image);

        return 0;
}

