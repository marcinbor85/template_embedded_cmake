set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

set(MCPU_FLAGS "-mthumb -mcpu=cortex-m4")
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(SPEC_FLAGS "-Wall -std=c99")
set(C_FLAGS "-fdata-sections -ffunction-sections -mlong-calls")
set(CPP_FLAGS "-fdata-sections -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls")
set(ASM_FLAGS "-x assembler-with-cpp")

set(LD_FLAGS "-T${CMAKE_CURRENT_LIST_DIR}/stm32f4_flash.ld -Wl,--gc-sections")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/arm-none-eabi.cmake)

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(hello_world
    VERSION 0.1.0 
    LANGUAGES C ASM
)

add_definitions(
    -DSTM32F40_41xxx
    -DUSE_STDPERIPH_DRIVER
)

include_directories(PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cmsis/core
    ${CMAKE_CURRENT_LIST_DIR}/cmsis/device
    ${CMAKE_CURRENT_LIST_DIR}/src
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libstm32)

add_executable(${PROJECT_NAME})
file(GLOB PROJECT_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.c ${CMAKE_CURRENT_LIST_DIR}/src/*.s)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBSTM32F4_NAME})

include(${CMAKE_CURRENT_LIST_DIR}/cmake/post-build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)
