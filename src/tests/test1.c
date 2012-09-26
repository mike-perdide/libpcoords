#include <stdio.h>
#include <picviz.h>

int main(void)
{
        pcimage_t *image;

        printf("Test1: create and destroy image\n");
        printf("===============================\n");
	picviz_engine_init();
        picviz_init(NULL, NULL);
        image = picviz_image_new();
        picviz_image_debug_printall(image);
        picviz_image_destroy(image);

        return 0;
}

