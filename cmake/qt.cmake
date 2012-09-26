
MACRO(QT4_AUTOMOC2 moc_SRCS)
  QT4_GET_MOC_INC_DIRS(_moc_INCS)

  SET(_matching_FILES )
  FOREACH (_current_FILE ${ARGN})

     GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
     # if "SKIP_AUTOMOC" is set to true, we will not handle this file here.
     # here. this is required to make bouic work correctly:
     # we need to add generated .cpp files to the sources (to compile them),
     # but we cannot let automoc handle them, as the .cpp files don't exist yet when
     # cmake is run for the very first time on them -> however the .cpp files might
     # exist at a later run. at that time we need to skip them, so that we don't add two
     # different rules for the same moc file
     GET_SOURCE_FILE_PROPERTY(_skip ${_abs_FILE} SKIP_AUTOMOC)

     IF ( NOT _skip AND EXISTS ${_abs_FILE} )

        FILE(READ ${_abs_FILE} _contents)

        GET_FILENAME_COMPONENT(_abs_PATH ${_abs_FILE} PATH)

        STRING(REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
        IF(_match)
            GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
            SET(_header ${_abs_PATH}/${_basename}.h)
            SET(_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)

            ADD_CUSTOM_COMMAND(OUTPUT ${_moc}
                COMMAND ${QT_MOC_EXECUTABLE}
                ARGS ${_moc_INCS} ${_header} -o ${_moc}
                DEPENDS ${_header}
            )

            ADD_FILE_DEPENDENCIES(${_abs_FILE} ${_moc})
            SET(${moc_SRCS} ${${moc_SRCS}} ${_moc})

        ENDIF(_match)
     ENDIF ( NOT _skip AND EXISTS ${_abs_FILE} )
  ENDFOREACH (_current_FILE)
ENDMACRO(QT4_AUTOMOC2)

MACRO(QT4_AUTOUIC)

  SET(_matching_FILES )
  FOREACH (_current_FILE ${ARGN})

     GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)

     IF ( EXISTS ${_abs_FILE} )

        FILE(READ ${_abs_FILE} _contents)

        GET_FILENAME_COMPONENT(_abs_PATH ${_abs_FILE} PATH)

        GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
        SET(_outfile ${CMAKE_CURRENT_SOURCE_DIR}/ui_${_basename}.h)
        SET(_header ${_basename}.h)
        SET(_source ${_basename}.cpp)

        ADD_CUSTOM_COMMAND(OUTPUT ${_outfile}
            COMMAND ${QT_UIC_EXECUTABLE}
            ARGS -o ${_outfile} ${_abs_FILE}
            MAIN_DEPENDENCY ${_abs_FILE})

        ADD_FILE_DEPENDENCIES(${_source} ${_outfile})

     ENDIF ( EXISTS ${_abs_FILE} )
  ENDFOREACH (_current_FILE)
ENDMACRO(QT4_AUTOUIC)

