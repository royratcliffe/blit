# Add custom commands for code analysis. Enable cppcheck for Debug build type.
# This will run cppcheck only if the build type is Debug.
find_program(CPPCHECK cppcheck)
if(CPPCHECK AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK}
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --output-file=${CMAKE_BINARY_DIR}/cppcheck_report.xml
            --xml
            --enable=all
            --enable=warning,style,performance,portability,information
            --suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cppcheck for code analysis"
        # Use VERBATIM to ensure the command is executed as written.
        # This is useful for handling paths with spaces or special characters.
        VERBATIM
    )
    set_property(TARGET cppcheck PROPERTY FOLDER "Analysis")
else()
    message(STATUS "Cppcheck not found or not enabled for Debug build type")
endif()
