#pragma once


// InitializeMemorySystem - initialize your memory system including your HeapManager and some FixedSizeAllocators
bool InitializeMemorySystem(void* i_pHeapMemory, const size_t& i_sizeHeapMemory);

// Collect - coalesce free blocks in attempt to create larger blocks
void Collect();

// DestroyMemorySystem - destroy your memory systems
void DestroyMemorySystem();

