enable_testing( )

add_executable( 
    test_fifo
    ${CMAKE_CURRENT_LIST_DIR}/test_fifo.c
    ${PROJECT_SOURCE_DIR}/src/app/utils/fifo.c
)

target_include_directories(
    test_fifo
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/src/app
)

add_test( test_fifo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fifo )

add_custom_target( check COMMAND ${CMAKE_CTEST_COMMAND} --verbose )
