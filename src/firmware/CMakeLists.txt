target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

file(
    GLOB PROJECT_SRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/app/*.c
    ${CMAKE_CURRENT_LIST_DIR}/bsp/*.c
    ${CMAKE_CURRENT_LIST_DIR}/hw/*.c
    ${CMAKE_CURRENT_LIST_DIR}/utils/*.c
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SRC_FILES}
)
