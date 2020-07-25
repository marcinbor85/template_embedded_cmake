if(NOT DEFINED PORT)
    message(FATAL_ERROR "Error: PORT not defined")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/port/${PORT}/toolchain-config.cmake)

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(
    hello_world
    VERSION 0.1.0 
    LANGUAGES C ASM
)

add_executable(${PROJECT_NAME})

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/port/${PORT}/
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

file(
    GLOB PROJECT_SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/hw/*.c
    ${PROJECT_SOURCE_DIR}/src/utils/*.c
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SRC_FILES}
)

target_link_libraries(${PROJECT_NAME})

include(${CMAKE_CURRENT_LIST_DIR}/cmake-utils/post-build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake-utils/utils.cmake)
