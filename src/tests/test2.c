#include <stdio.h>
#include <pcoords.h>

int main(void)
{
        struct pcimage_t *image;
        struct axis_t *axis;

        printf("Test2: create and destroy image and axis\n");
        printf("========================================\n");
	pcoords_engine_init();
        pcoords_init(NULL, NULL);
        image = pcoords_image_new();
        axis = pcoords_axis_new();
        pcoords_image_axis_append(image, axis);
        pcoords_image_debug_printall(image);
        pcoords_image_destroy(image);

        return 0;
}

