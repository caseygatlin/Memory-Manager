#include "DynamicMemory.h"
#include "FixedSizeAllocatorInitData.h"
#include "HeapManagerProxy.h"
#include "MemorySystem.h"
#include <assert.h>

class HeapManager;
class FixedSizeAllocator;

extern HeapManager* S_DEFAULT_HEAP_MANAGER;
extern FixedSizeAllocator* S_FIXED_SIZE_ALLOCATORS[5];
extern size_t S_NUM_FIXED_SIZE_ALLOCATORS;

bool InitializeMemorySystem(void* i_pHeapMemory, size_t i_sizeHeapMemory)
{
	S_DEFAULT_HEAP_MANAGER = CreateDefaultHeap(i_pHeapMemory, i_sizeHeapMemory);
	assert(S_DEFAULT_HEAP_MANAGER);

	for (unsigned int i = 0; i < S_NUM_FSA_INIT; i++)
	{ 
		CreateFixedSizeAllocator(S_FSA_INITS[i].sizeBlock, S_FSA_INITS[i].numBlocks, S_DEFAULT_HEAP_MANAGER);
	}

	return true;
}

void Collect()
{
	HeapManagerProxy::Collect(S_DEFAULT_HEAP_MANAGER);
}

void DestroyMemorySystem()
{
	size_t numFSA = S_NUM_FIXED_SIZE_ALLOCATORS;

	for (int j = 0; j < numFSA; j++)
	{
		HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, S_FIXED_SIZE_ALLOCATORS[j]);
		S_NUM_FIXED_SIZE_ALLOCATORS--;
	}


	HeapManagerProxy::Destroy(S_DEFAULT_HEAP_MANAGER);
	S_DEFAULT_HEAP_MANAGER = nullptr;
	
	for (int j = 0; j < 5; j++)
	{
		S_FIXED_SIZE_ALLOCATORS[j] = nullptr;
	}
}


