#include <stdio.h>
#include <picviz.h>

int main(void)
{
        struct pcimage_t *image;
        struct axis_t *axis;

        printf("Test2: create and destroy image and axis\n");
        printf("========================================\n");
	picviz_engine_init();
        picviz_init(NULL, NULL);
        image = picviz_image_new();
        axis = picviz_axis_new();
        picviz_image_axis_append(image, axis);
        picviz_image_debug_printall(image);
        picviz_image_destroy(image);

        return 0;
}

