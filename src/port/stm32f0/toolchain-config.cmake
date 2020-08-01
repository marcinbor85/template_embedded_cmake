set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0)

set(MCPU_FLAGS "-mthumb -mcpu=cortex-m0")
set(VFP_FLAGS "-mfloat-abi=soft")
set(SPEC_FLAGS "-Wall -std=c99")
set(C_FLAGS "-fdata-sections -ffunction-sections -mlong-calls")
set(CPP_FLAGS "-fdata-sections -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls")
set(ASM_FLAGS "-x assembler-with-cpp")

set(LD_FLAGS "-T${CMAKE_CURRENT_LIST_DIR}/stm32f0_flash.ld -Wl,--gc-sections")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake-toolchains/arm-none-eabi.cmake)
