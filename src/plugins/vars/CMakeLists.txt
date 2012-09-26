	include_directories(${LIBPICVIZ_SOURCE_DIR}/src/include)
link_directories(${LIBPICVIZ_BUILD_DIR})

# Debug plugin
add_library(picvizvardebug MODULE vardebug.c)
target_link_libraries(picvizvardebug picviz)
install(TARGETS picvizvardebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

