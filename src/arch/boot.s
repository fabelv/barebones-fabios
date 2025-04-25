.set ALIGN,    1<<0		    # 0b0001 
.set MEMINFO,  1<<1		    # 0b0010
# ------------------------------
# Multiboot header (GRUB uses this to find and load the kernel)
# ------------------------------
.set FLAGS,    ALIGN | MEMINFO      # Request page alignment
.set MAGIC,    0x1BADB002           # Required magic number for multiboot
.set CHECKSUM, -(MAGIC + FLAGS)	    # Checksum so sum of all three is zero
				    # CHECKSUM = -(0x1BADB002 + 0x00000001) => -0x1BSFB003

#3.1.1 The layout of Multiboot header
#The layout of the Multiboot header must be as follows:
#Offset	Type	Field Name	Note
#0	u32	magic		required
#4	u32	flags		required
#8	u32	checksum	required
#12	u32	header_addr	if flags[16] is set
#16	u32	load_addr	if flags[16] is set
#20	u32	load_end_addr	if flags[16] is set
#24	u32	bss_end_addr	if flags[16] is set
#28	u32	entry_addr	if flags[16] is set
#32	u32	mode_type	if flags[2] is set
#36	u32	width		if flags[2] is set
#40	u32	height		if flags[2] is set
#44	u32	depth		if flags[2] is set
#https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-layout

.section .multiboot                 # Place header in its own section
.align 4                            # Ensure 4-byte alignment
.long MAGIC                         # Multiboot magic value
.long FLAGS                         # Flags for GRUB
.long CHECKSUM                      # Checksum (MAGIC + FLAGS + CHECKSUM == 0)

# ------------------------------
# Stack setup (16 KiB, uninitialized)
# ------------------------------
.section .bss
.align 16                           # Align the stack to 16 bytes
stack_bottom:                       
.skip 16384                         # Reserve 16 KiB of stack space
stack_top:                          # Label top of the stack

# ------------------------------
# Entry point
# ------------------------------
.section .text
.global _start                      # Make _start visible to linker
.type _start, @function             # Declare _start as a function symbol

_start:
    mov $stack_top, %esp            # Initialize stack pointer
    call kernel_main                # Jump into C++ kernel code

    cli                             # Disable interrupts
1:  hlt                             # Halt the CPU
    jmp 1b                          # Infinite loop (label '1' going back)

.size _start, . - _start            # Set symbol size for debugging tools

