#pragma once

#define __TRACK_ALLOCATIONS

#include "DynamicMemory.h"
#include "HeapManager.h"
#include <assert.h>
#include <stdio.h>




namespace HeapManagerProxy
{

	inline HeapManager * CreateHeapManager(const void * i_pMemory, const size_t& i_sizeMemory)
	{
		return HeapManager::create(i_pMemory, i_sizeMemory);
	}

	inline void Destroy(HeapManager * i_pManager)
	{
		assert(i_pManager);

		i_pManager->destroy();
	}

	inline void * alloc(HeapManager * i_pManager, const size_t& i_size)
	{
		assert(i_pManager);

		return i_pManager->_alloc(i_size);
	}

	inline void * alloc(HeapManager * i_pManager, const size_t& i_size, const unsigned int& i_alignment)
	{
		assert(i_pManager);

		return i_pManager->_alloc(i_size, i_alignment);
	}

	inline bool free(HeapManager * i_pManager, void * i_ptr)
	{
		assert(i_pManager);

		return i_pManager->_free(i_ptr);
	}

	inline void Collect(HeapManager * i_pManager)
	{
		assert(i_pManager);

		i_pManager->collect();
	}


	inline bool Contains(const HeapManager * i_pManager, const void * i_ptr)
	{
		assert(i_pManager);

		return i_pManager->Contains(i_ptr);
	}

	inline bool IsAllocated(const HeapManager * i_pManager, const void * i_ptr)
	{
		assert(i_pManager);

		return i_pManager->IsAllocated(i_ptr);
	}

	inline size_t GetLargestFreeBlock(const HeapManager * i_pManager)
	{
		assert(i_pManager);

		return i_pManager->getLargestFreeBlock();
	}

	inline size_t GetTotalFreeMemory(const HeapManager * i_pManager)
	{
		assert(i_pManager);

		return i_pManager->getTotalFreeMemory();
	}

	inline void ShowFreeBlocks(const HeapManager * i_pManager)
	{
		assert(i_pManager);

		i_pManager->ShowFreeBlocks();
	}

	inline void ShowOutstandingAllocations(const HeapManager * i_pManager)
	{
		assert(i_pManager);

#ifdef __TRACK_ALLOCATIONS
		i_pManager->ShowOutstandingAllocations();
#else
		printf("HeapManager compiled without __TRACK_ALLOCATIONS defined.\n");
#endif

	}

} // namespace HeapManagerProxy