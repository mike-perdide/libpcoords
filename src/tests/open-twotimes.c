#include <stdio.h>

#include <picviz.h>

#define PGDL_FILE "test-simple.pgdl"

int main(void)
{
  PicvizImage *image;
  PicvizImage *image2;
  

  picviz_engine_init();
  engine.debug = 3;
  picviz_init(NULL, NULL);

  printf("***** Round 1\n");
  image = (PicvizImage *)pcv_parse(PGDL_FILE, NULL);
  picviz_render_image(image);
  picviz_image_debug_printall(image);
  picviz_image_destroy(image);

  printf("***** Round 2\n");
  image2 = (PicvizImage *)pcv_parse(PGDL_FILE, NULL);
  picviz_render_image(image2);
  picviz_image_debug_printall(image2);
  picviz_image_destroy(image2);
  
  return 0;
}
