cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(NOT DEFINED TESTS)
    if(NOT DEFINED PORT)
        message(FATAL_ERROR "Error: PORT not defined")
    endif()

    include(${CMAKE_SOURCE_DIR}/src/port/${PORT}/toolchain-config.cmake)
endif()

project(
    hello_world
    VERSION 0.1.0 
    LANGUAGES C ASM
)

if(NOT DEFINED TESTS)
    add_subdirectory(
        ${CMAKE_SOURCE_DIR}/src/
    )

else()
    enable_testing()

    add_subdirectory(
        ${CMAKE_SOURCE_DIR}/tests/
    )
endif()
