set(AVRDUDE avrdude)

add_custom_target(
    flash
    COMMAND ${AVRDUDE} -p atmega328p -c usbasp -U flash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin
    DEPENDS ${PROJECT_NAME}.bin
)
