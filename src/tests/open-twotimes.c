#include <stdio.h>

#include <pcoords.h>

#define PGDL_FILE "test-simple.pgdl"

int main(void)
{
  PcoordsImage *image;
  PcoordsImage *image2;
  

  pcoords_engine_init();
  engine.debug = 3;
  pcoords_init(NULL, NULL);

  printf("***** Round 1\n");
  image = (PcoordsImage *)pcv_parse(PGDL_FILE, NULL);
  pcoords_render_image(image);
  pcoords_image_debug_printall(image);
  pcoords_image_destroy(image);

  printf("***** Round 2\n");
  image2 = (PcoordsImage *)pcv_parse(PGDL_FILE, NULL);
  pcoords_render_image(image2);
  pcoords_image_debug_printall(image2);
  pcoords_image_destroy(image2);
  
  return 0;
}
