	include_directories(${LIBPCOORDS_SOURCE_DIR}/src/include)
link_directories(${LIBPCOORDS_BUILD_DIR})

# Debug plugin
add_library(pcoordsvardebug MODULE vardebug.c)
target_link_libraries(pcoordsvardebug pcoords)
install(TARGETS pcoordsvardebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

