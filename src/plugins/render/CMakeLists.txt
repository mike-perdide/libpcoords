include_directories(${LIBPICVIZ_SOURCE_DIR}/src/include)
link_directories(${LIBPICVIZ_BUILD_DIR})

# Debug plugin
#add_library(picvizrendebug MODULE rendebug.c)
##set_target_properties(picvizrendebug PROPERTIES SOVERSION 1)
#target_link_libraries(picvizrendebug picviz)
#install(TARGETS picvizrendebug LIBRARY DESTINATION ${MOD_INSTALL_DIR})

# Heatmap plugin
add_library(picvizrenheatline MODULE renheatline.c)
#set_target_properties(picvizrenheatline PROPERTIES SOVERSION 1)
target_link_libraries(picvizrenheatline picviz)
install(TARGETS picvizrenheatline LIBRARY DESTINATION ${MOD_INSTALL_DIR})
