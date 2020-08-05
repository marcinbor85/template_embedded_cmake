# Template Embedded CMake
This is advanced template embedded project.

## Features
* cmake as build system
* layered firmware architecture
* strictly separated hardware layer for easy porting
* common firmware source code for all implemented ports
* example ports for AVR and STM32
* no external libraries dependencies
* few useful libraries for user interface and communication
* build-in and configured unit tests

## Configuration examples

Configuration for unit tests:

```bash
mkdir build
mkdir build/tests
cd build/tests
cmake ../.. -DTESTS=1
make
make test
```

Configuration for firmware build:

```bash
mkdir build
mkdir build/src
cd build/src
cmake ../.. -DPORT=stm32f4      # configure port STM32F4
cmake ../.. -DPORT=stm32f0      # configure port STM32F0
cmake ../.. -DPORT=atmega328p   # configure port ATMEGA328P
make
make flash
```
