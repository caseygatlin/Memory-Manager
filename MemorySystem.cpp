#include "MemorySystem.h"

struct FixedSizeAllocatorInitData
{
	size_t sizeBlock;
	size_t numBlocks;
};

bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	//pDefaultHeap = CreateDefaultHeap();
	//assert(pDefaultMap);

	//for (unsigned int i = 0; i < numFSAs; i++)
	//{ CreateFixedSizeAllocator(FSASizes[i].sizeBlock, FSASizes[i].numBlocks, pDefaultHeap);}

	// create your HeapManager and FixedSizeAllocators
	return true;
}

void Collect()
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager
}

void DestroyMemorySystem()
{
	// Destroy your HeapManager and FixedSizeAllocators
}

