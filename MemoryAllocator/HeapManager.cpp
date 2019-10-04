#include <assert.h>
#include "HeapManager.h"
#include "BlockDesc.h"
#include <iostream>

const size_t GUARD_BANDING = 4;


HeapManager::HeapManager(void* i_pHeapMemory, size_t i_heapMemorySize, unsigned int i_numDescriptors)
{
	// Record the current and max numbers of descriptors
	m_maxNumDesc = i_numDescriptors;
	m_numDesc = 1;

	// Assign location of the FreeMemHead in the HeapManager
	char* c_FreeMemHead = static_cast<char*>(i_pHeapMemory) + i_heapMemorySize - i_numDescriptors * sizeof(BlockDesc);
	void* v_FreeMemHead = static_cast<void*>(c_FreeMemHead);
	m_pFreeMemHead = static_cast<BlockDesc*>(v_FreeMemHead);

	// Assign values of first BlockDescr in FreeMem
	m_pFreeMemHead->m_pBlockBase = i_pHeapMemory;
	m_pFreeMemHead->m_pNext = nullptr;
	m_pFreeMemHead->m_sizeBlock = i_heapMemorySize;

	// Define empty UsedMem
	m_pUsedMemHead = nullptr;
}

HeapManager* HeapManager::create(void* i_pHeapMemory, size_t i_heapMemorySize, unsigned int i_numDescriptors)
{
	HeapManager* pManager = static_cast<HeapManager*>(i_pHeapMemory);
	*pManager = HeapManager(i_pHeapMemory, i_heapMemorySize, i_numDescriptors);
	return pManager;
}

void* HeapManager::_alloc(size_t i_bytes)
{
	// Make sure we don't go over our number of descriptors
	assert(m_numDesc < m_maxNumDesc);

	// Adjust to have 4-byte alignment
	size_t bytesToAlloc = i_bytes;
	while (bytesToAlloc % 4 != 0)
	{
		bytesToAlloc++;
	}
	
	// Add guard bands
	bytesToAlloc += GUARD_BANDING * 2;

	// Initialize return value
	void* pReturn = nullptr;

	// Search for big enough block, create new block in UsedMem when found
	BlockDesc* pFree = m_pFreeMemHead;
	while (pFree != nullptr)
	{
		// If current block is big enough, allocate memory
		if (pFree->m_sizeBlock >= i_bytes)
		{
			BlockDesc* pUsed = m_pUsedMemHead;
			BlockDesc* pUsedNew = nullptr;
			if (pUsed != nullptr)
			{
				while (pUsed->m_pNext != nullptr)
				{
					pUsed = pUsed->m_pNext;
				}

				// Create new BlockDesc in UsedMem
				pUsed->m_pNext = m_pFreeMemHead + m_numDesc * sizeof(BlockDesc);
				pUsedNew = pUsed->m_pNext;
			}
			else
			{
				pUsed = m_pFreeMemHead + m_numDesc * sizeof(BlockDesc);
				pUsedNew = pUsed;
			}

			// Create new BlockDesc in UsedMem
			pUsedNew = m_pFreeMemHead + m_numDesc * sizeof(BlockDesc);

			// Assign the BlockDesc base address
			pUsedNew->m_pBlockBase = pFree->m_pBlockBase;

			// Assign the BlockDesc size
			pUsedNew->m_sizeBlock = bytesToAlloc;

			// Assign next ptr
			pUsedNew->m_pNext = nullptr;


			// Assigns new block base to BlockDesc in FreeMem
			char* c_pFreeBlockBase = static_cast<char*>(pFree->m_pBlockBase) + bytesToAlloc;
			void* v_pFreeBlockBase = static_cast<void*>(c_pFreeBlockBase);
			pFree->m_pBlockBase = v_pFreeBlockBase;

			// Adjusts size of BlockDesc in FreeMem
			pFree->m_sizeBlock -= bytesToAlloc;

			// Assign return pointer and adjust for guard banding
			char* c_pReturn = static_cast<char*>(pUsedNew->m_pBlockBase) + GUARD_BANDING;
			pReturn = static_cast<void*>(c_pReturn);

			//Exit loop and return
			return pReturn;

		}

		// Otherwise move to next block
		pFree = pFree->m_pNext;
	}

	// If no block big enough is found, return nullptr
	return nullptr;
}

void* HeapManager::_alloc(size_t i_bytes, unsigned int i_alignment)
{
	return nullptr; //TODO: Implement alloc alignment
}

bool HeapManager::_free(void* i_ptr)
{
	return false; //TODO: implement free
}

void HeapManager::collect()
{
	return; //TODO: implement collect
}

size_t HeapManager::getLargestFreeBlock()
{
	return 0; //TODO: implement get largest free block
}

size_t HeapManager::getTotalFreeMemory()
{
	return 0; //TODO: implement get total free memory
}

bool HeapManager::Contains(void* i_ptr)
{
	return false; //TODO: implement contains
}

bool HeapManager::IsAllocated(void* i_ptr)
{
	return false; //TODO: implement is allocated
}

void HeapManager::ShowFreeBlocks()
{
	int blockCount = 1;
	BlockDesc* pFree = m_pFreeMemHead;
	while (pFree != nullptr)
	{
		std::cout << "Block " << blockCount << ":\t" << pFree << std::endl;
		std::cout << "\tSize:\t" << pFree->m_sizeBlock << std::endl;
		std::cout << "\tNext:\t" << pFree->m_pNext << std::endl;
		blockCount++;
		pFree = pFree->m_pNext;
	}
}

void HeapManager::ShowOutstandingAllocations()
{
	int blockCount = 1;
	BlockDesc* pUsed = m_pUsedMemHead;
	while (pUsed != nullptr)
	{
		std::cout << "Block " << blockCount << ":\t" << pUsed << std::endl;
		std::cout << "\tSize:\t" << pUsed->m_sizeBlock << std::endl;
		std::cout << "\tNext:\t" << pUsed->m_pNext << std::endl;
		blockCount++;
		pUsed = pUsed->m_pNext;
	}
}

void HeapManager::destroy()
{
	return; //TODO: implement destroy
}