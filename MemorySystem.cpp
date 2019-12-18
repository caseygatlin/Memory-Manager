#include "MemorySystem.h"
#include "FixedSizeAllocator.h"
#include "HeapManagerProxy.h"
#include "FSAInitData.h"
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



	bool InitializeMemorySystem(void* i_pHeapMemory, size_t i_sizeHeapMemory)
	{
		S_DEFAULT_HEAP_MANAGER = CreateDefaultHeap(i_pHeapMemory, i_sizeHeapMemory);
		assert(S_DEFAULT_HEAP_MANAGER);

		for (unsigned int i = 0; i < S_NUM_FSA_INIT; i++)
		{ 
			HeapManagerProxy::CreateFixedSizeAllocator(S_FSA_INITS[i].sizeBlock, S_FSA_INITS[i].numBlocks, S_DEFAULT_HEAP_MANAGER);
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
		HeapManagerProxy::Destroy(S_DEFAULT_HEAP_MANAGER);
	}


}
