cmake_minimum_required(VERSION 3.25)
project(blit)

add_library(blit
    # Source files for the blit library.
    ${CMAKE_CURRENT_SOURCE_DIR}/src/blit/rgn1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/blit/rop.c
)

# Include directories for the library.
target_include_directories(blit
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:include>
)
