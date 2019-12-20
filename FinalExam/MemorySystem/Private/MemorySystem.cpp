#include "DynamicMemory.h"
#include "FixedSizeAllocatorInitData.h"
#include "HeapManagerProxy.h"
#include "MemorySystem.h"
#include <assert.h>



class HeapManager;
class FixedSizeAllocator;


// External variables that hold the memory managers
extern HeapManager*         S_DEFAULT_HEAP_MANAGER;
extern FixedSizeAllocator*  S_FIXED_SIZE_ALLOCATORS[3];
extern size_t               S_NUM_FIXED_SIZE_ALLOCATORS;



// Creates a Default Heap Manager and an array of FSA's
bool InitializeMemorySystem(void* i_pHeapMemory, size_t i_sizeHeapMemory)
{

    // Create Default Heap Manager
	S_DEFAULT_HEAP_MANAGER = CreateDefaultHeap(i_pHeapMemory, i_sizeHeapMemory);
	assert(S_DEFAULT_HEAP_MANAGER);


    // Create an array of FSA's
	for (unsigned int i = 0; i < S_NUM_FSA_INIT; i++)
	{ 

		CreateFixedSizeAllocator(S_FSA_INITS[i].sizeBlock, S_FSA_INITS[i].numBlocks, S_DEFAULT_HEAP_MANAGER);

	}

	return true;

}



// Coalesce free memory in Default Heap Manager
void Collect()
{

	HeapManagerProxy::Collect(S_DEFAULT_HEAP_MANAGER);

}



// Destruct FSA's and Default Heap Manager
void DestroyMemorySystem()
{

    // Free FSA's
	size_t numFSA = S_NUM_FIXED_SIZE_ALLOCATORS;

	for (int j = 0; j < numFSA; j++)
	{

		HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, S_FIXED_SIZE_ALLOCATORS[j]);

		S_NUM_FIXED_SIZE_ALLOCATORS--;

	}


    // Destruct Default Heap Manager
	HeapManagerProxy::Destroy(S_DEFAULT_HEAP_MANAGER);


    // Nullify Default Heap Manager
	S_DEFAULT_HEAP_MANAGER = nullptr;
	

    // Nullify FSA's
	for (int j = 0; j < numFSA; j++)
	{

		S_FIXED_SIZE_ALLOCATORS[j] = nullptr;

	}

}


