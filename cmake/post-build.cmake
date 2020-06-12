add_custom_target(${PROJECT_NAME}.hex DEPENDS ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}.bin DEPENDS ${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${SIZE} ARGS ${PROJECT_NAME}
    COMMENT "Summary"
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJCOPY} ARGS -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMENT "Generate Intel HEX file"
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJCOPY} ARGS -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    COMMENT "Generate Binary file"
)
