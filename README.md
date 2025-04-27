# barebones-fabios

This project follows the [Bare Bones](https://wiki.osdev.org/Bare_Bones) tutorial from the OSDev wiki.

The goal is to build a simple operating system kernel using a GCC cross-compiler (`i686-elf`) and run it using QEMU.

## Roadmap

The following roadmap outlines the planned progression of the project:

### Multiboot Compliant Boot
- Correct Multiboot header
- Pass and validate Multiboot information

### Basic VGA Text Output
- Print text to screen
- Implement scrolling and simple cursor movement

### Multiboot Information Handling
- Parse memory map
- Parse boot modules and command line if present

### Memory Management
- Physical memory manager (simple page allocator)
- Reserve kernel and multiboot data regions

### Interrupt Handling
- Set up basic IDT (Interrupt Descriptor Table)
- Handle CPU exceptions (e.g., division by zero)

### Testing Infrastructure
- Separate test kernel builds
- Unit test core components (memory, VGA, logging)

### Shell Interface
- Implement a basic input system
- Execute simple commands (e.g., `meminfo`, `clear`, `reboot`)

### Further Enhancements (Optional)
- Dynamic memory allocation (heap)
- Basic task switching and timer interrupts
- Early file system exploration (initial ramdisk)
