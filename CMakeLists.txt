cmake_minimum_required(VERSION 3.12)

# Set the path to the directory containing the pico_sdk_import.cmake file
set(PICO_SDK_IMPORT_PATH "/path/to/directory/")

# Include the pico_sdk_import.cmake file from the specified path
include(${PICO_SDK_IMPORT_PATH}/pico_sdk_import.cmake)

# If in the project folder use this
#include(pico_sdk_import.cmake)

project(neopixel_fading)

pico_sdk_init()

add_executable(neopixel_fading)

pico_generate_pio_header(neopixel_fading ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_sources(neopixel_fading PRIVATE neopixel_fading.c)

pico_enable_stdio_usb(neopixel_fading 1)
pico_enable_stdio_uart(neopixel_fading 1)

target_link_libraries(neopixel_fading PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(neopixel_fading)
