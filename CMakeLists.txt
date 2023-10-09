cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(my_project)
pico_sdk_init()

add_executable(cube
    src/main.cpp src/pixels.cpp src/pixels.h src/accel.cpp src/accel.h
)

pico_generate_pio_header(cube
        ${CMAKE_CURRENT_LIST_DIR}/src/neopixel.pio
)

pico_enable_stdio_usb(cube 1)
pico_enable_stdio_uart(cube 0)

target_link_libraries(cube pico_stdlib hardware_i2c hardware_pio hardware_dma)
target_include_directories(cube PRIVATE src)
pico_add_extra_outputs(cube)
