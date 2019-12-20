#include "BitArray.h"
#include "DynamicMemory.h"
#include "FixedSizeAllocator.h"
#include "FixedSizeAllocatorInitData.h"
#include "HeapManagerProxy.h"
#include <assert.h>


// Defines preprocessor constants depending on 64 or 32 bit architecture
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
class FixedSizeAllocator;


// Define external variables that hold the memory managers
extern HeapManager* S_DEFAULT_HEAP_MANAGER              = nullptr;
extern FixedSizeAllocator* S_FIXED_SIZE_ALLOCATORS[3]   = { nullptr, nullptr, nullptr };
extern size_t S_NUM_FIXED_SIZE_ALLOCATORS               = 0;



// Creates and constructs an FSA for a certain size block
void CreateFixedSizeAllocator(const size_t& i_sizeBlock, const size_t& i_numBlocks, HeapManager* i_pHeapManager)
{

    // Correctly size the bitArray
	size_t bitArraySize = i_numBlocks / BIT_DIVISOR;

	if (i_numBlocks % BIT_DIVISOR > 0)
	{

		bitArraySize++;

	}


    // Calculate memory needed
	size_t   bitArrayMemorySize   = sizeof(BitArray)           + bitArraySize       * BIT_DIVISOR;
	size_t   FSAMemorySize        = sizeof(FixedSizeAllocator) + bitArrayMemorySize + i_sizeBlock * i_numBlocks;


    // Allocate memory
	void* pFSAMemoryLocation = HeapManagerProxy::alloc(i_pHeapManager, FSAMemorySize);


    // Construct an FSA at that memory location
	S_FIXED_SIZE_ALLOCATORS[S_NUM_FIXED_SIZE_ALLOCATORS] = FixedSizeAllocator::Create(pFSAMemoryLocation, FSAMemorySize, i_numBlocks, i_sizeBlock);
	S_NUM_FIXED_SIZE_ALLOCATORS++;

}



// Creates, constructs, and returns the Default Heap Manager
HeapManager* CreateDefaultHeap(void* i_pHeapMemory, const size_t& i_sizeHeapMemory)
{

	if (S_DEFAULT_HEAP_MANAGER == nullptr)
	{

		assert(i_pHeapMemory);

        // Create and return Heap Manager
		HeapManager*   pReturnHeap = HeapManagerProxy::CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory);
		return         pReturnHeap;

	}

	return nullptr;

}



// Returns Default Heap Manager
HeapManager* GetDefaultHeapManager()
{
	return S_DEFAULT_HEAP_MANAGER;
}



// Find and return a fixed size allocator that should allocate a certain size block
FixedSizeAllocator* FindFixedSizeAllocator(const size_t& i_sizeBlock)
{
    // Cycle through the FSA's and return one if possible
    for (size_t i = 0; i < S_NUM_FSA_INIT; i++)
    {

        if (S_FIXED_SIZE_ALLOCATORS[i]->GetBlockSize() == i_sizeBlock)
        {

            return S_FIXED_SIZE_ALLOCATORS[i];

        }

    }

    return nullptr;

}
