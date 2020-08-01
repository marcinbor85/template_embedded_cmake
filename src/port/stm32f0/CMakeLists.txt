target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    -DSTM32F072xB
    -DREVERSE_USER_BUTTON
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/cmsis/core
    ${CMAKE_CURRENT_LIST_DIR}/cmsis/device
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
