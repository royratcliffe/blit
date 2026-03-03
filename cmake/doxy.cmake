# https://cmake.org/cmake/help/latest/module/FindDoxygen.html
# https://www.mcternan.me.uk/mscgen/
find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
if(DOXYGEN_FOUND)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
    set(DOXYGEN_USE_MATHJAX YES)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_EXCLUDE ${CMAKE_BINARY_DIR})
    doxygen_add_docs(${PROJECT_NAME}_doxy ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen for ${PROJECT_NAME}"
    )
    set_target_properties(${PROJECT_NAME}_doxy PROPERTIES FOLDER "Documentation")
endif()
