#include <picviz.h>

int main(void)
{
        struct pcimage_t *image;

	picviz_engine_init();
        picviz_init(NULL, NULL);
        image = (PicvizImage *)pcv_parse("test-colors.pcv", NULL);
/*         image_to_svg(image); */
        return 0;
}
