#include <stdio.h>
#include <picviz.h>

int main(void)
{
        struct pcimage_t *image;
        struct axis_t *axis;
        struct line_t *line;
        struct axisplot_t *axisplot;

        printf("Test3: create lines\n");
        printf("===================\n");
	picviz_engine_init();
        picviz_init(NULL, NULL);
        image = picviz_image_new();
        axis = picviz_axis_new();
/*         picviz_image_axis_append(image, axis); */
/*         line = picviz_line_new(); */
/*         axisplot = picviz_axisplot_new(); */
/*         axisplot->y = 42; */
/*         axisplot->axis_id = 0; */
/*         picviz_line_axisplot_append(line, axisplot); */
/*         axisplot = picviz_axisplot_new(); */
/*         axisplot->y = 12.43; */
/*         axisplot->axis_id = 1; */
/*         picviz_line_axisplot_append(line, axisplot); */
/*         picviz_image_line_append(image, line); */

        picviz_image_debug_printall(image);
        picviz_image_destroy(image);

        return 0;
}

