set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME})

add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/port/${PORT}/
)

add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/firmware/
)

target_link_libraries(${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/cmake-utils/post-build.cmake)
