# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(display)

# Define the executable
add_executable(display display.c)

# Link any necessary libraries
target_link_libraries(display m)
