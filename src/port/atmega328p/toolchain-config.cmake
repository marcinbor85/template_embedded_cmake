set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(MCPU_FLAGS "-mmcu=atmega328p")
set(VFP_FLAGS "")
set(SPEC_FLAGS "-Wall -std=c99")
set(C_FLAGS "-fdata-sections -ffunction-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields")
set(CPP_FLAGS "-fdata-sections -ffunction-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-rtti -fno-exceptions")
set(ASM_FLAGS "-x assembler-with-cpp")

set(LD_FLAGS "-Wl,--gc-sections")

set(CMAKE_SYSTEM_INCLUDE_PATH "/usr/avr/include" )
set(CMAKE_SYSTEM_LIBRARY_PATH "/usr/avr/lib" )

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake-toolchains/avr-gcc.cmake)


