#include <picviz.h>

int main(void)
{
        struct pcimage_t *image;

	picviz_engine_init();
        picviz_init(NULL, NULL);
        image = (struct pcimage_t *)pcv_parse("test4.pcv", NULL);
//        picviz_image_debug_printall(image);

/*         image_to_svg(image); */

        return 0;
}
