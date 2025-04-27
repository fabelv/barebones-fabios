cmake_minimum_required(VERSION 3.23)
project(barebones_kernel C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only build for the kernel
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(CMAKE_C_COMPILER i686-elf-gcc)
set(CMAKE_CXX_COMPILER i686-elf-g++)

# Freestanding flags
set(KERNEL_CXX_FLAGS
  -ffreestanding -fno-exceptions -fno-rtti -O2
)

# Sources
file(GLOB_RECURSE KERNEL_SOURCES
  src/kernel/*.cpp
  src/core/memory/*.cpp
  src/core/vga/*.cpp
  src/lib/*.cpp
)

add_library(kernel_objs OBJECT ${KERNEL_SOURCES})

target_compile_options(kernel_objs PRIVATE ${KERNEL_CXX_FLAGS})
target_include_directories(kernel_objs PRIVATE src)

