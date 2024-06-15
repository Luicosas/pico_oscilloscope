cmake_minimum_required(VERSION 3.28)

if (NOT DEFINED PICO_SDK_PATH) 
  set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../../pico-sdk")
endif()
message(STATUS "PICO_SDK_PATH: ${PICO_SDK_PATH}")

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(probe)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(probe probe.c)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(probe pico_stdlib hardware_gpio hardware_adc hardware_dma)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(probe)

pico_enable_stdio_usb(probe 1)
pico_enable_stdio_uart(probe 0)