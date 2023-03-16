# DynamicBlockArray

Highly efficient dynamic memory structure with fast memory acces and light resize.

The stucture allocate the memory in chunks and we keep track of these in a table, thus making it possible to acces the memory directly (without any jumps like in a linked list).

# What does it provide?

- Allocation
- Deallocation
- Indexing
- Resize
- Read (through blocks)
- Write (through blocks)

# Future possible functionalities?

- Get base pointer
- Cursor
- Get cursor
