#include <pcoords.h>

int main(void)
{
        struct pcimage_t *image;

	pcoords_engine_init();
        pcoords_init(NULL, NULL);
        image = (PcoordsImage *)pcv_parse("test-colors.pcv", NULL);
/*         image_to_svg(image); */
        return 0;
}
