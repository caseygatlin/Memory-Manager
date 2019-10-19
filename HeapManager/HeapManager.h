#pragma once

#include "BlockDesc.h"


//Manages dynamic allocation of memory given a chunk of memory
class HeapManager
{

public:

	HeapManager(void* i_pHeapMemory, size_t i_heapMemorySize);

	static HeapManager* create(void* i_pHeapMemory, size_t i_heapMemorySize);

	static const size_t	s_MinumumToLeave = sizeof(BlockDesc) + 1;


	// allocates memory. named with a underscore to resolve name clash
	void*	_alloc(size_t i_bytes);
	// allocation with alignment. returns a block of memory with a given alignment
	void*	_alloc(size_t i_bytes, unsigned int i_alignment);

	// frees an allocation
	bool	_free(void* i_ptr);

	// attempt to merge abutting blocks.
	void	collect();

	// tells us the size of the largest free block
	size_t	getLargestFreeBlock();
	// tells us how much total memory is available
	size_t	getTotalFreeMemory();

	// tells us if a pointer is within this HeapManagers managed memory
	bool	Contains( void * i_ptr);
	// tells us if this pointer is an outstanding allocation from this block
	bool	IsAllocated(void* i_ptr);

	// a debugging helper function to show us all the free blocks.
	void	ShowFreeBlocks();
	// a debugging helper function to show us all the outstanding blocks.
	void	ShowOutstandingAllocations();

	// Destructs the heap manager
	void	destroy();

private:
	BlockDesc* m_pFreeMemHead;
	BlockDesc* m_pUsedMemHead;
};
