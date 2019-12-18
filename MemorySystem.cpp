#include "MemorySystem.h"
#include "FixedSizeAllocator.h"
#include "HeapManagerProxy.h"
#include <assert.h>


namespace MemorySystemProxy
{
	


	FixedSizeAllocator* FindFixedSizeAllocator(const size_t& i_sizeBlock)
	{
		return HeapManagerProxy::GetFixedAllocator(i_sizeBlock, S_DEFAULT_HEAP_MANAGER);
	}



	HeapManager* CreateDefaultHeap(void* i_pHeapMemory, size_t i_sizeHeapMemory)
	{
		if (S_DEFAULT_HEAP_MANAGER == nullptr)
		{
			assert(i_pHeapMemory);
			
			HeapManager* pReturnHeap = HeapManagerProxy::CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory);

			return pReturnHeap;
		}

		return nullptr;
	}
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


}
