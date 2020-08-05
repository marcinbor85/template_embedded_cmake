set(STFLASH st-flash)

add_custom_target(
    flash
    COMMAND ${STFLASH} --format=ihex write ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_NAME}.hex
)
