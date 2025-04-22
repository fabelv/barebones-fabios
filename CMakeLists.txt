cmake_minimum_required(VERSION 3.23)
project(bare_bones_kernel C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Options ===
option(KERNEL_BUILD "Build the kernel" ON)
option(KERNEL_TEST_BUILD "Build the kernel with in-kernel tests" ON)

# === Enable testing with GTest when not cross-compiling ===
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

# === Only set the cross-compiler for kernel builds ===
if(KERNEL_BUILD OR KERNEL_TEST_BUILD)
  set(CMAKE_SYSTEM_NAME Generic)
  set(CMAKE_SYSTEM_PROCESSOR i686)

  set(CMAKE_C_COMPILER i686-elf-gcc)
  set(CMAKE_CXX_COMPILER i686-elf-g++)
  set(CMAKE_ASM_COMPILER i686-elf-gcc)
endif()

# === Common compiler/linker flags ===
set(KERNEL_CXX_FLAGS
  -ffreestanding
  -fno-exceptions
  -fno-rtti
  -O2
  -std=c++20
)
set(KERNEL_LINK_FLAGS
  -T ${CMAKE_SOURCE_DIR}/src/linker.ld
  -nostdlib
  -ffreestanding
  -lgcc
)

# === Kernel Sources ===
file(GLOB_RECURSE CORE_SOURCES src/core/*.cpp)
file(GLOB_RECURSE LIB_SOURCES  src/lib/*.cpp)

set(ARCH_ASM src/arch/boot.s)

set(KERNEL_SRC
  src/kernel.cpp
  ${CORE_SOURCES}
  ${LIB_SOURCES}
  ${ARCH_ASM}
)

set(KERNEL_TEST_SRC
  src/tests/kernel_test.cpp
  ${CORE_SOURCES}
  ${LIB_SOURCES}
  ${ARCH_ASM}
)

# === Production Kernel Build ===
if(KERNEL_BUILD)
  add_executable(kernel.elf ${KERNEL_SRC})
  set_target_properties(kernel.elf PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  target_compile_options(kernel.elf PRIVATE ${KERNEL_CXX_FLAGS})
  target_link_options(kernel.elf PRIVATE ${KERNEL_LINK_FLAGS})
endif()

# === Test Kernel Build ===
if(KERNEL_TEST_BUILD)
  add_executable(kernel_test.elf ${KERNEL_TEST_SRC})
  set_target_properties(kernel_test.elf PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  target_compile_options(kernel_test.elf PRIVATE ${KERNEL_CXX_FLAGS})
  target_link_options(kernel_test.elf PRIVATE ${KERNEL_LINK_FLAGS})
endif()

# === ISO Targets ===
add_custom_target(kernel.iso ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/isodir/boot/grub
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/kernel.elf ${CMAKE_BINARY_DIR}/isodir/boot/fabios.bin
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config/grub.cfg ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
  COMMAND grub-mkrescue -o ${CMAKE_BINARY_DIR}/fabios.iso ${CMAKE_BINARY_DIR}/isodir
  DEPENDS kernel.elf
)

add_custom_target(kernel_test.iso
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/isodir/boot/grub
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/kernel_test.elf ${CMAKE_BINARY_DIR}/isodir/boot/fabios.bin
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config/grub.cfg ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
  COMMAND grub-mkrescue -o ${CMAKE_BINARY_DIR}/fabios-test.iso ${CMAKE_BINARY_DIR}/isodir
  DEPENDS kernel_test.elf
)

# === Run Targets ===
add_custom_target(run
  COMMAND qemu-system-i386 -cdrom ${CMAKE_BINARY_DIR}/fabios.iso
  DEPENDS kernel.iso
)

add_custom_target(run_tests
  COMMAND qemu-system-i386 -cdrom ${CMAKE_BINARY_DIR}/fabios-test.iso
  DEPENDS kernel_test.iso
)

