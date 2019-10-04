#pragma once

namespace HeapManagerController
{
	// Manages allocation and deallocation of memory in the heap
	class HeapManager
	{
	public:
		static HeapManager* create(void* i_pHeapMemory, size_t i_heapMemorySize);

		// Allocates a given number of bytes in memory and returns address
		void* memAllocate(size_t i_numBytes);

		// Frees memory for given address
		void* memFree(void* i_ptr);

		// Returns whether memory is allocated for the given ptr
		bool isAllocated(void* i_ptr);

	private:
		void* m_pHeapMemory;
	};
}
