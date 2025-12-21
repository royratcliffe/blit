cmake_minimum_required(VERSION 3.25)
project(blit)

add_library(blit
    # Source files for the blit library.
    ${CMAKE_CURRENT_SOURCE_DIR}/src/blit/rgn1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/blit/rop2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/blit/phase_align.c
)

# Include directories for the library.
target_include_directories(blit
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:include>
)

# Add a CTest executable for running all tests.
# This will be used to run the tests defined in the test sources.
# The test sources will be compiled into a test executable.
include(CTest)
create_test_sourcelist(test_sources
    test_runner.c
    tests/pat.c
)

# Add a test executable that links against the library.
# The test executable will be built from the test sources.
# It will include the library and any necessary test files.
add_executable(test_runner
    ${test_sources}
)
target_link_libraries(test_runner PRIVATE blit)

add_test(NAME pat COMMAND test_runner tests/pat)
