#include "DynamicMemory.h"
#include "FixedSizeAllocator.h"
#include "BitArray.h"
#include "FixedSizeAllocatorInitData.h"
#include "HeapManagerProxy.h"
#include <assert.h>

#ifdef _WIN64
#ifndef BIT_DIVISOR
#define BIT_DIVISOR 64
#endif // !BIT_DIVISOR
#else
#ifndef BIT_DIVISOR
#define BIT_DIVISOR 32
#endif // !BIT_DIVISOR
#endif // _WIN64


class HeapManager;

FixedSizeAllocator* FindFixedSizeAllocator(const size_t& i_sizeBlock)
{

	for (size_t i = 0; i < S_NUM_FSA_INIT; i++)
	{

		if (S_FIXED_SIZE_ALLOCATORS[i]->GetBlockSize() == i_sizeBlock)
		{

			return S_FIXED_SIZE_ALLOCATORS[i];

		}

	}

	return nullptr;

}



void CreateFixedSizeAllocator(const size_t& i_sizeBlock, const size_t& i_numBlocks, HeapManager* i_pHeapManager)
{

	size_t bitArraySize = i_numBlocks / BIT_DIVISOR;

	if (i_numBlocks % BIT_DIVISOR > 0)
	{

		bitArraySize++;

	}


	size_t   bitArrayMemorySize   = sizeof(BitArray)           + bitArraySize       * BIT_DIVISOR;
	size_t   FSAMemorySize        = sizeof(FixedSizeAllocator) + bitArrayMemorySize + i_sizeBlock * i_numBlocks;
	void* pFSAMemoryLocation = HeapManagerProxy::alloc(i_pHeapManager, FSAMemorySize);


	S_FIXED_SIZE_ALLOCATORS[S_NUM_FIXED_SIZE_ALLOCATORS] = FixedSizeAllocator::Create(pFSAMemoryLocation, FSAMemorySize, i_numBlocks, i_sizeBlock);

	S_NUM_FIXED_SIZE_ALLOCATORS++;

}



HeapManager* CreateDefaultHeap(void* i_pHeapMemory, size_t i_sizeHeapMemory)
{

	if (S_DEFAULT_HEAP_MANAGER == nullptr)
	{

		assert(i_pHeapMemory);

		HeapManager*   pReturnHeap = HeapManagerProxy::CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory);
		return         pReturnHeap;

	}

	return nullptr;

}

HeapManager* GetDefaultHeapManager()
{
	return S_DEFAULT_HEAP_MANAGER;
}

