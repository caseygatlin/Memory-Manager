#pragma once

namespace memory_system
{
	namespace dynamic_memory
	{
		class FixedSizeAllocator;
		class HeapManager;

		static HeapManager*				S_DEFAULT_HEAP_MANAGER = nullptr;
		static FixedSizeAllocator*		S_FIXED_SIZE_ALLOCATORS[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
		static size_t					S_NUM_FIXED_SIZE_ALLOCATORS = 0;

		void					CreateFixedSizeAllocator(const size_t& i_sizeBlock, const size_t& i_numBlocks, HeapManager* i_pHeapManager);
		HeapManager*			CreateDefaultHeap(void* i_pHeapMemory, size_t i_sizeHeapMemory);
		FixedSizeAllocator*		FindFixedSizeAllocator(const size_t& i_sizeBlock);

	}
}