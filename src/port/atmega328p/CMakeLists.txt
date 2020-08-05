target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    -D__AVR_ATmega328P__
    -DREVERSE_USER_BUTTON
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/atmega
)

file(
    GLOB PORT_SRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.s
    ${CMAKE_CURRENT_LIST_DIR}/system/*.c
    ${CMAKE_CURRENT_LIST_DIR}/hw/*.c
    ${CMAKE_CURRENT_LIST_DIR}/utils/*.c
    ${CMAKE_CURRENT_LIST_DIR}/bsp/*.c
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${PORT_SRC_FILES}
)

include(${CMAKE_SOURCE_DIR}/cmake-utils/avr-utils.cmake)
