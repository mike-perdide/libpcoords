include_directories(${LIBPICVIZ_SOURCE_DIR}/src/include)
link_directories(${LIBPICVIZ_BUILD_DIR}/)

add_library(picvizoutsvg MODULE outsvg.c)
add_library(picvizoutdebug MODULE outdebug.c)
target_link_libraries(picvizoutsvg picviz)
target_link_libraries(picvizoutdebug picviz)
install(TARGETS picvizoutdebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

install(TARGETS picvizoutsvg LIBRARY DESTINATION ${MOD_INSTALL_DIR})
# install(TARGETS picvizoutdebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

# # CSV plugin
# add_library(picvizoutcsv MODULE outcsv.c)
# target_link_libraries(picvizoutcsv picviz)
# install(TARGETS picvizoutcsv LIBRARY DESTINATION ${MOD_INSTALL_DIR})

# # Picviz plugin
# add_library(picvizoutpcv MODULE outpcv.c)
# target_link_libraries(picvizoutpcv picviz)
# install(TARGETS picvizoutpcv LIBRARY DESTINATION ${MOD_INSTALL_DIR})

#
# PNG cairo plugin
#
if(Cairo_FOUND)
include_directories(APPEND ${Cairo_INCLUDE_DIRS})
add_library(picvizoutpngcairo MODULE outpngcairo.c)
target_link_libraries(picvizoutpngcairo picviz ${Cairo_LIBRARIES})
install(TARGETS picvizoutpngcairo LIBRARY DESTINATION ${MOD_INSTALL_DIR})
endif(Cairo_FOUND)

#
# Frequency sorted values
#
add_library(picvizoutfreqsort MODULE outfreqsort.c)
target_link_libraries(picvizoutfreqsort picviz)
install(TARGETS picvizoutfreqsort LIBRARY DESTINATION ${MOD_INSTALL_DIR})

#
# Text
#
add_library(picvizouttext MODULE outtext.c)
target_link_libraries(picvizouttext picviz)
install(TARGETS picvizouttext LIBRARY DESTINATION ${MOD_INSTALL_DIR})
