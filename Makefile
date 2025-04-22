CMAKE_BUILD_DIR := build/kernel

.PHONY: all test run run_tests iso kernel_test.iso kernel_test clean

all:
	cmake -B $(CMAKE_BUILD_DIR) -S .
	cmake --build $(CMAKE_BUILD_DIR) --target kernel.elf

iso: all
	cmake --build $(CMAKE_BUILD_DIR) --target kernel.iso

run: iso
	cmake --build $(CMAKE_BUILD_DIR) --target run

test:
	cmake -B build/test -S .
	cmake --build build/test --target unit_tests
	./build/test/unit_tests

kernel_test:
	cmake -B $(CMAKE_BUILD_DIR) -S .
	cmake --build $(CMAKE_BUILD_DIR) --target kernel_test.elf

kernel_test.iso: kernel_test
	cmake --build $(CMAKE_BUILD_DIR) --target kernel_test.iso

run_tests: kernel_test.iso
	cmake --build $(CMAKE_BUILD_DIR) --target run_tests

clean:
	rm -rf build


