include_directories(${LIBPCOORDS_SOURCE_DIR}/gnulib/ ${LIBPCOORDS_SOURCE_DIR})

add_library(glthread STATIC cond.c  lock.c  thread.c  threadlib.c  tls.c)

