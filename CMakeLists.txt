cmake_minimum_required(VERSION 3.28)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(pico_oscilloscope)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(pico_oscilloscope main.c)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(pico_oscilloscope pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(pico_oscilloscope)
