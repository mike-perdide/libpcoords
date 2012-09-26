include_directories(${LIBPICVIZ_SOURCE_DIR}/gnulib/ ${LIBPICVIZ_SOURCE_DIR})

add_library(glthread STATIC cond.c  lock.c  thread.c  threadlib.c  tls.c)

