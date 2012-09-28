#include <stdio.h>
#include <pcoords.h>

int main(void)
{
        pcimage_t *image;

        printf("Test1: create and destroy image\n");
        printf("===============================\n");
	pcoords_engine_init();
        pcoords_init(NULL, NULL);
        image = pcoords_image_new();
        pcoords_image_debug_printall(image);
        pcoords_image_destroy(image);

        return 0;
}

