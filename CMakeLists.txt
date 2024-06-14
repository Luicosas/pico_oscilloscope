cmake_minimum_required(VERSION 3.28)

if (NOT DEFINED PICO_SDK_PATH) 
  set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../pico-sdk/")
endif()
message(STATUS "PICO_SDK_PATH: ${PICO_SDK_PATH}")

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_oscilloscope)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(pico_oscilloscope main.c)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(pico_oscilloscope pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(pico_oscilloscope)
