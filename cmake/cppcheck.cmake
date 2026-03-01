# Add custom commands for code analysis. Enable cppcheck for Debug build type.
# This will run cppcheck only when building the Debug configuration, using configuration-aware logic.
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
        COMMAND $<$<CONFIG:Debug>:${CPPCHECK}>
            $<$<CONFIG:Debug>:--project=${CMAKE_BINARY_DIR}/compile_commands.json>
            $<$<CONFIG:Debug>:--output-file=${CMAKE_BINARY_DIR}/cppcheck_report.xml>
            $<$<CONFIG:Debug>:--xml>
            $<$<CONFIG:Debug>:--enable=all>
            $<$<CONFIG:Debug>:--enable=warning,style,performance,portability,information>
            $<$<CONFIG:Debug>:--suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt>
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cppcheck for code analysis (Debug configuration only)"
        # Use VERBATIM to ensure the command is executed as written.
        # This is useful for handling paths with spaces or special characters.
        VERBATIM
    )
    set_property(TARGET cppcheck PROPERTY FOLDER "Analysis")
else()
    message(STATUS "Cppcheck not found; code analysis target will not be created")
endif()
