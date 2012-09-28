include_directories(${LIBPCOORDS_SOURCE_DIR}/src/include)
link_directories(${LIBPCOORDS_BUILD_DIR})

# Debug plugin
#add_library(pcoordsrendebug MODULE rendebug.c)
##set_target_properties(pcoordsrendebug PROPERTIES SOVERSION 1)
#target_link_libraries(pcoordsrendebug pcoords)
#install(TARGETS pcoordsrendebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

# Heatmap plugin
add_library(pcoordsrenheatline MODULE renheatline.c)
#set_target_properties(pcoordsrenheatline PROPERTIES SOVERSION 1)
target_link_libraries(pcoordsrenheatline pcoords)
install(TARGETS pcoordsrenheatline LIBRARY DESTINATION ${MOD_INSTALL_DIR})
