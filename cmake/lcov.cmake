# Add custom commands for code coverage analysis. Enable lcov and genhtml if
# they are found. Only enable code coverage analysis for Debug build type, as it
# is typically used for development and testing. Code coverage analysis can add
# overhead to the build and runtime, so it is generally not recommended for
# Release builds.
#
# If lcov is found, add the coverage compiler and linker flags to enable code
# coverage analysis.
#
# Note that find_program will return the path to the executable if found, or
# NOTFOUND if not found. It will also cache the result, so subsequent calls will
# return the cached value. This allows us to check if lcov is available and
# conditionally add code coverage targets.
find_program(LCOV lcov)
if(LCOV AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add the necessary compiler and linker flags to enable code coverage analysis.
    foreach(target IN ITEMS blit test_runner)
        target_compile_options(${target} PRIVATE --coverage)
        target_link_options(${target} PRIVATE --coverage)
    endforeach()

    # Add custom targets for code coverage analysis using lcov and genhtml.
    # These targets will run the tests, capture coverage data, and generate an HTML report.
    add_custom_target(ctest
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS test_runner
    )
    add_custom_target(lcov
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${LCOV} --capture --directory . --output-file coverage.info
        COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/test*' --output-file coverage_filtered.info
    )
    set_property(TARGET lcov PROPERTY FOLDER "Analysis")

    # If genhtml is found, add a custom target to generate an HTML report from the
    # filtered coverage data. This will create a coverage_report directory with an
    # index.html file that can be opened in a web browser to view the coverage report.
    find_program(GENHTML genhtml)
    if(GENHTML)
        message(STATUS "lcov and genhtml found, code coverage targets will be available")
        add_custom_target(genhtml
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated in: ${CMAKE_BINARY_DIR}/coverage_report/index.html"
            DEPENDS lcov
        )
        set_property(TARGET genhtml PROPERTY FOLDER "Analysis")
    else()
        message(STATUS "genhtml not found, HTML code coverage report target will not be available")
    endif()
endif()
