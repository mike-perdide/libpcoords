add_subdirectory(plugins)
#add_subdirectory(bindings)

include_directories(${LIBPCOORDS_SOURCE_DIR}/src/include ${PCRE_INCLUDE_DIR} ${LIBPCOORDS_SOURCE_DIR}/gnulib/glthread ${LIBPCOORDS_SOURCE_DIR}/libev ${LIBPCOORDS_BINARY_DIR}/src)


link_directories(
  ${LIBPCOORDS_BINARY_DIR}/libev
)
#set(CMAKE_SHARED_LINKER_FLAGS "-lm -lfl -ly")
#link_directories(${LIBPCOORDS_BINARY_DIR}/ ${LIBPCOORDS_BINARY_DIR}/gnulib/glthread ${LIBPCOORDS_BINARY_DIR}/libev)

IF(PCRE_FOUND)
    INCLUDE_DIRECTORIES(AFTER ${PCRE_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(pcoords ${PCRE_LIBRARIES})
ENDIF(PCRE_FOUND)


# Parser
BISON_TARGET(pcoords ${LIBPCOORDS_SOURCE_DIR}/src/pgdl-parser.y ${LIBPCOORDS_BINARY_DIR}/src/pgdl-parser.c)
FLEX_TARGET(pcoords ${LIBPCOORDS_SOURCE_DIR}/src/pgdl-lexer.l ${LIBPCOORDS_BINARY_DIR}/src/pgdl-lexer.c)

# Filter
BISON_TARGET(pcoords ${LIBPCOORDS_SOURCE_DIR}/src/filter.yac.y ${LIBPCOORDS_BINARY_DIR}/src/filter-parser.c)
FLEX_TARGET(pcoords ${LIBPCOORDS_SOURCE_DIR}/src/filter.lex.l ${LIBPCOORDS_BINARY_DIR}/src/filter-lexer.c)

add_library(pcoords SHARED axis.c axisplot.c correlation.c debug.c real-time.c filter.c image.c learn.c line.c pgdl.c plugins.c engine.c pcoords-hash.c pcoords-init.c pcoords-pcre.c properties.c render.c values-mapping.c variable.c utils.c filter-parser.c filter-lexer.c pgdl-parser.c pgdl-lexer.c prop-color.c)

set_target_properties(pcoords PROPERTIES SOVERSION 2)

#target_link_libraries(pcoords "-lm -ldl -L${LIBPCOORDS_BINARY_DIR}/libev -lev")
target_link_libraries(pcoords "-lm -ldl -L${LIBPCOORDS_BINARY_DIR}/libev/ -lev")

install(TARGETS pcoords LIBRARY DESTINATION ${LIB_INSTALL_DIR})
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
		include/pcoords.h
		include/pcoords-hash.h
		include/pcoords-init.h
		include/pcoords-pcre.h
		include/pcoords-stats.h
		include/plugins.h
		include/prop-color.h
		include/properties.h
		include/render.h
		include/types.h
		include/values-mapping.h
		include/variable.h
		include/utils.h
	DESTINATION include/pcoords
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

