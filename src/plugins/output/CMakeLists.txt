include_directories(${LIBPCOORDS_SOURCE_DIR}/src/include)
link_directories(${LIBPCOORDS_BUILD_DIR}/)

add_library(pcoordsoutsvg MODULE outsvg.c)
add_library(pcoordsoutdebug MODULE outdebug.c)
target_link_libraries(pcoordsoutsvg pcoords)
target_link_libraries(pcoordsoutdebug pcoords)
install(TARGETS pcoordsoutdebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

install(TARGETS pcoordsoutsvg LIBRARY DESTINATION ${MOD_INSTALL_DIR})
# install(TARGETS pcoordsoutdebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

# # CSV plugin
# add_library(pcoordsoutcsv MODULE outcsv.c)
# target_link_libraries(pcoordsoutcsv pcoords)
# install(TARGETS pcoordsoutcsv LIBRARY DESTINATION ${MOD_INSTALL_DIR})

# # Pcoords plugin
# add_library(pcoordsoutpcv MODULE outpcv.c)
# target_link_libraries(pcoordsoutpcv pcoords)
# install(TARGETS pcoordsoutpcv LIBRARY DESTINATION ${MOD_INSTALL_DIR})

#
# PNG cairo plugin
#
if(Cairo_FOUND)
include_directories(APPEND ${Cairo_INCLUDE_DIRS})
add_library(pcoordsoutpngcairo MODULE outpngcairo.c)
target_link_libraries(pcoordsoutpngcairo pcoords ${Cairo_LIBRARIES})
install(TARGETS pcoordsoutpngcairo LIBRARY DESTINATION ${MOD_INSTALL_DIR})
endif(Cairo_FOUND)

#
# Frequency sorted values
#
add_library(pcoordsoutfreqsort MODULE outfreqsort.c)
target_link_libraries(pcoordsoutfreqsort pcoords)
install(TARGETS pcoordsoutfreqsort LIBRARY DESTINATION ${MOD_INSTALL_DIR})

#
# Text
#
add_library(pcoordsouttext MODULE outtext.c)
target_link_libraries(pcoordsouttext pcoords)
install(TARGETS pcoordsouttext LIBRARY DESTINATION ${MOD_INSTALL_DIR})
