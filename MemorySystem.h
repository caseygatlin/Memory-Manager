#pragma once

class FixedSizeAllocator;
class HeapManager;

namespace MemorySystemProxy
{
	static HeapManager* S_DEFAULT_HEAP_MANAGER;

	// InitializeMemorySystem - initialize your memory system including your HeapManager and some FixedSizeAllocators
	bool InitializeMemorySystem(void* i_pHeapMemory, size_t i_sizeHeapMemory);

	FixedSizeAllocator* FindFixedSizeAllocator(const size_t& i_sizeBlock);

	// Collect - coalesce free blocks in attempt to create larger blocks
	void Collect();

	// DestroyMemorySystem - destroy your memory systems
	void DestroyMemorySystem();
}
