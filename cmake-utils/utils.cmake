set(STFLASH st-flash)

add_custom_target(
    flash
    COMMAND ${STFLASH} --format=ihex write ${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_NAME}.hex
)
