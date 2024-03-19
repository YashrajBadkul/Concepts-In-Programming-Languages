Dynamic Memory Management

This program demonstrates dynamic memory management techniques in C, implementing a custom memory allocator similar to malloc and free functions. The program efficiently allocates and deallocates memory space from a predefined memory pool.


Features
Manual Memory Allocation:

Provides malloc_manual function for allocating memory space.
Offers free_manual function for deallocating memory.
Memory Pool:

Utilizes a predefined memory pool (free_space) for allocation.
Enhances memory management efficiency.
Memory Division:

Implements memory division strategy to allocate appropriate memory size.
Optimizes memory usage by dividing larger blocks.
Memory Merging:

Merges adjacent free memory blocks to prevent memory fragmentation.
Ensures efficient memory utilization.
Error Handling:

Proper error messages are displayed for out-of-bounds memory access.
Code Overview
malloc_manual: Allocates memory space manually based on the required size.
free_manual: Deallocates memory space manually.
Initialise: Initializes the memory pool and sets up the initial metadata.
divide: Divides a memory block into two parts.
merge_blocks: Merges adjacent free memory blocks to prevent fragmentation.
Limitations
Memory allocation is limited to the predefined memory pool size (FREE_SPACE_SIZE).
Does not support memory reallocation (realloc).