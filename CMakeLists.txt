cmake_minimum_required(VERSION 3.26)

include(pico_sdk_import.cmake)

project(tccLibs)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(SimpleCollections/cmake)
add_subdirectory(TaskManagerIO/cmake)
add_subdirectory(IoAbstraction/cmake)
add_subdirectory(tcMenu/cmake)
add_subdirectory(tcUnicodeHelper/cmake)
add_subdirectory(LiquidCrystalIO/cmake)
add_subdirectory(AdafruitGFXNativePort)

IF(NOT DEFINED "ENV{TC_CMAKE_EXCLUDE_EXAMPLES}")
    add_subdirectory(examples/UsingCollections)
    add_subdirectory(examples/UsingTaskManager)
    add_subdirectory(examples/UsingIoAbstraction)
    add_subdirectory(examples/Using1602I2C)
    add_subdirectory(examples/UsingSSD1306Oled)
    add_subdirectory(examples/ControlAppOledEncoderPico)
ENDIF ()