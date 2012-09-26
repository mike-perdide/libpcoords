add_subdirectory(plugins)
#add_subdirectory(bindings)

include_directories(${LIBPICVIZ_SOURCE_DIR}/src/include ${PCRE_INCLUDE_DIR} ${LIBPICVIZ_SOURCE_DIR}/gnulib/glthread ${LIBPICVIZ_SOURCE_DIR}/libev)


link_directories(
  ${LIBPICVIZ_BINARY_DIR}/libev
)
#set(CMAKE_SHARED_LINKER_FLAGS "-lm -lfl -ly")
#link_directories(${LIBPICVIZ_BINARY_DIR}/ ${LIBPICVIZ_BINARY_DIR}/gnulib/glthread ${LIBPICVIZ_BINARY_DIR}/libev)

IF(PCRE_FOUND)
    INCLUDE_DIRECTORIES(AFTER ${PCRE_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(picviz ${PCRE_LIBRARIES})
ENDIF(PCRE_FOUND)


# Parser
BISON_TARGET(picviz ${LIBPICVIZ_SOURCE_DIR}/src/pgdl-parser.y ${LIBPICVIZ_BINARY_DIR}/src/pgdl-parser.c)
FLEX_TARGET(picviz ${LIBPICVIZ_SOURCE_DIR}/src/pgdl-lexer.l ${LIBPICVIZ_BINARY_DIR}/src/pgdl-lexer.c)

# Filter
BISON_TARGET(picviz ${LIBPICVIZ_SOURCE_DIR}/src/filter.yac.y ${LIBPICVIZ_BINARY_DIR}/src/filter-parser.c)
FLEX_TARGET(picviz ${LIBPICVIZ_SOURCE_DIR}/src/filter.lex.l ${LIBPICVIZ_BINARY_DIR}/src/filter-lexer.c)

add_library(picviz SHARED axis.c axisplot.c correlation.c debug.c real-time.c filter.c image.c learn.c line.c plugins.c engine.c picviz-hash.c picviz-init.c picviz-pcre.c properties.c render.c values-mapping.c variable.c utils.c filter-parser.c filter-lexer.c pgdl-parser.c pgdl-lexer.c prop-color.c)

set_target_properties(picviz PROPERTIES SOVERSION 2)

#target_link_libraries(picviz "-lm -ldl -L${LIBPICVIZ_BINARY_DIR}/libev -lev")
target_link_libraries(picviz "-lm -ldl -L${LIBPICVIZ_BINARY_DIR}/libev/ -lev")

install(TARGETS picviz LIBRARY DESTINATION ${LIB_INSTALL_DIR})
install(FILES
		include/axis.h
		include/axisplot.h
		include/common.h
		include/correlation.h
		include/debug.h
		include/defaults.h
		include/draw.h
		include/engine.h
		include/real-time.h
		include/filter.h
		include/image.h
		include/learn.h
		include/line.h
		include/linuxlist.h
		include/pcimage.h
		include/pcv-parser.h
		include/picviz.h
		include/picviz-hash.h
		include/picviz-init.h
		include/picviz-pcre.h
		include/picviz-stats.h
		include/plugins.h
		include/prop-color.h
		include/properties.h
		include/render.h
		include/types.h
		include/values-mapping.h
		include/variable.h
		include/utils.h
	DESTINATION include/picviz
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
