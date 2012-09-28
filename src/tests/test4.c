#include <pcoords.h>

int main(void)
{
        struct pcimage_t *image;

	pcoords_engine_init();
        pcoords_init(NULL, NULL);
        image = (struct pcimage_t *)pcv_parse("test4.pcv", NULL);
//        pcoords_image_debug_printall(image);

/*         image_to_svg(image); */

        return 0;
}
