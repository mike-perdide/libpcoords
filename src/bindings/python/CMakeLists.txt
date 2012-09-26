if(ENABLE_python)

ADD_CUSTOM_COMMAND(TARGET picviz POST_BUILD COMMAND python ./setup.py install)

endif(ENABLE_python)

