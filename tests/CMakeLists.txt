enable_testing( )

add_executable( 
    test_fifo
    ${CMAKE_CURRENT_LIST_DIR}/test_fifo.c
    ${PROJECT_SOURCE_DIR}/src/firmware/utils/fifo.c
)

target_include_directories(
    test_fifo
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/src/firmware
)

add_test( test_fifo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fifo )

add_executable( 
    test_slip
    ${CMAKE_CURRENT_LIST_DIR}/test_slip.c
    ${PROJECT_SOURCE_DIR}/src/firmware/utils/slip.c
)

target_include_directories(
    test_slip
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/src/firmware
)

add_test( test_slip ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_slip )

add_custom_target( check COMMAND ${CMAKE_CTEST_COMMAND} --verbose )
