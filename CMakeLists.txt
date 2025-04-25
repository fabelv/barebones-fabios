cmake_minimum_required(VERSION 3.23)
project(bare_bones_kernel C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(KERNEL_BUILD "Build the kernel" ON)
option(KERNEL_TEST_BUILD "Build the kernel with in-kernel tests" ON)

# === GTest for unit tests (host-side only) ===
if(NOT CMAKE_CROSSCOMPILING)
  enable_testing()
  find_package(GTest REQUIRED)

  file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
  file(GLOB_RECURSE LIB_SOURCES src/lib/*.cpp)

  add_executable(unit_tests
    ${TEST_SOURCES}
    ${LIB_SOURCES}
  )

  target_include_directories(unit_tests PRIVATE src)
  target_link_libraries(unit_tests GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(unit_tests)
endif()

# === Cross compiler settings ===
if(KERNEL_BUILD OR KERNEL_TEST_BUILD)
  set(CMAKE_SYSTEM_NAME Generic)
  set(CMAKE_SYSTEM_PROCESSOR i686)

  set(CMAKE_C_COMPILER i686-elf-gcc)
  set(CMAKE_CXX_COMPILER i686-elf-g++)
endif()

# === Kernel object file targets ===
set(KERNEL_CXX_FLAGS
  -ffreestanding -fno-exceptions -fno-rtti -O2 -std=c++20
)

file(GLOB_RECURSE CORE_SOURCES src/core/*.cpp)
file(GLOB_RECURSE LIB_SOURCES src/lib/*.cpp)

if(KERNEL_BUILD)
  add_library(kernel_objs OBJECT src/kernel.cpp ${CORE_SOURCES} ${LIB_SOURCES})
  target_compile_options(kernel_objs PRIVATE ${KERNEL_CXX_FLAGS})
  target_include_directories(kernel_objs PRIVATE ${CMAKE_SOURCE_DIR}/src)
endif()

if(KERNEL_TEST_BUILD)
  add_library(kernel_test_objs OBJECT src/tests/kernel_test.cpp ${CORE_SOURCES} ${LIB_SOURCES})
  target_compile_options(kernel_test_objs PRIVATE ${KERNEL_CXX_FLAGS})
  target_include_directories(kernel_test_objs PRIVATE ${CMAKE_SOURCE_DIR}/src)
endif()

