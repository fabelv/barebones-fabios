ENTRY(_start)

SECTIONS
{
  /* 
   * Start the kernel at 1 MiB (0x00100000), which is the traditional load address
   * for Multiboot kernels. This ensures the Multiboot header is located within
   * the first 8 KiB of the file, as required by the Multiboot specification.
   */
  . = 1M;

  /* 
   * Code section: contains the Multiboot header and executable code
   */
  .text BLOCK(4K) : ALIGN(4K) {
    KEEP(*(.multiboot))  /* Place Multiboot header early so GRUB can find it */
    *(.text)       /* All executable code */
  }

  /* 
   * Read-only data: constant strings, lookup tables, etc.
   */
  .rodata BLOCK(4K) : ALIGN(4K) {
    *(.rodata)
  }

  /* 
   * Writable data: global/static variables that are initialized
   */
  .data BLOCK(4K) : ALIGN(4K) {
    *(.data)
  }

  /* 
   * Uninitialized data (BSS): global/static variables that start with zero
   */
  .bss BLOCK(4K) : ALIGN(4K) {
    __bss_start = .;     /* Start of BSS — useful for clearing memory at boot */
    *(.bss)
    *(COMMON)            /* Uninitialized common symbols (old C style) */
    __bss_end = .;       /* End of BSS */
  }
}

