#pragma once

#include "BlockDesc.h"

namespace memory_system
{
	namespace dynamic_memory
	{
		//Manages dynamic allocation of memory given a chunk of memory
		class HeapManager
		{

		public:

			HeapManager(const void* i_pHeapMemory, const size_t& i_heapMemorySize);

			static HeapManager* Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize);

			static const size_t	s_MinumumToLeave = sizeof(BlockDesc) + 1;


			// allocates memory. named with a underscore to resolve name clash
			void* _alloc(const size_t& i_bytes);
			// allocation with alignment. returns a block of memory with a given alignment
			void* _alloc(const size_t& i_bytes, const unsigned int& i_alignment);

			// frees an allocation
			bool	_free(void* i_ptr);

			// attempt to merge abutting blocks.
			void	Collect();

			// tells us the size of the largest free block
			size_t	GetLargestFreeBlock() const;
			// tells us how much total memory is available
			size_t	GetTotalFreeMemory() const;

			// tells us if a pointer is within this HeapManagers managed memory
			bool	Contains(const void* i_ptr) const;
			// tells us if this pointer is an outstanding allocation from this block
			bool	IsAllocated(const void* i_ptr) const;

			// a debugging helper function to show us all the free blocks.
			void	ShowFreeBlocks() const;
			// a debugging helper function to show us all the outstanding blocks.
			void	ShowOutstandingAllocations() const;

			// Destructs the heap manager
			void	Destroy();

		private:
			BlockDesc* m_pFreeMemHead;
			BlockDesc* m_pUsedMemHead;
		};

	}
}

