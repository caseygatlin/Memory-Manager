#include <assert.h>
#include "HeapManager.h"
#include "BlockDesc.h"
#include <iostream>
#include <stdint.h>

// 4 byte guard bands in case of writing over allocated block
const size_t GUARD_BANDING = 4;


HeapManager::HeapManager(void* i_pHeapMemory, size_t i_heapMemorySize)
{
	// Calculate start of heap memory
	uintptr_t*	uip_pHeapMemory		= static_cast<uintptr_t*>	(i_pHeapMemory);
	void*		v_pHeapStart		= static_cast<void*>		(uip_pHeapMemory + sizeof(BlockDesc));

	// Define FreeMem
	m_pFreeMemHead					= static_cast<BlockDesc*>(i_pHeapMemory);
	m_pFreeMemHead->m_pBlockBase	= v_pHeapStart;
	m_pFreeMemHead->m_pPrev			= nullptr;
	m_pFreeMemHead->m_pNext			= nullptr;
	m_pFreeMemHead->m_sizeBlock		= i_heapMemorySize - sizeof(BlockDesc);

	// Define empty UsedMem
	m_pUsedMemHead					= nullptr;
}



// Inizializes new HeapManager
HeapManager* HeapManager::create(void* i_pHeapMemory, size_t i_heapMemorySize)
{
	// Defines new HeapManager in memory
	HeapManager*	pManager		= static_cast<HeapManager*>	(i_pHeapMemory);


	// Constructs heap manager
	uintptr_t*		ip_pHeapMemory	= static_cast<uintptr_t*>	(i_pHeapMemory);
	void*			v_pHeapStart	= static_cast<void*>		(ip_pHeapMemory + sizeof(HeapManager));
					*pManager		= HeapManager(v_pHeapStart, i_heapMemorySize);

	return pManager;
}



// Allocates a given number of bytes and returns memory address
void* HeapManager::_alloc(size_t i_bytes)
{
	// Adjust to 4-byte alignment
	size_t bytesToAlloc = i_bytes;
	while (bytesToAlloc % 4 != 0)
	{
		bytesToAlloc++;
	}
	

	// Add guard bands
	bytesToAlloc	+= GUARD_BANDING * 2;
	bytesToAlloc	+= sizeof(BlockDesc);


	// Initialize return value
	void* pReturn	= nullptr;


	// Search for big enough block,
	// create new block in UsedMem when found
	BlockDesc* pFree = m_pFreeMemHead;
	while (pFree != nullptr)
	{

		// If current block is big enough, allocate memory
		if (pFree->m_sizeBlock >= bytesToAlloc)
		{
			BlockDesc* pUsed		= m_pUsedMemHead;
			BlockDesc* pUsedNew		= nullptr;

			// If UsedMem has at least one item
			if (pUsed != nullptr)
			{

				// Iterate till 2nd to last item
				while (pUsed->m_pNext != nullptr)
				{
					pUsed = pUsed->m_pNext;
				}


				// Create new BlockDesc in UsedMem
				pUsed->m_pNext		= static_cast<BlockDesc*>(pFree->m_pBlockBase);
				pUsedNew			= pUsed->m_pNext;
				pUsedNew->m_pPrev	= pUsed;

			}

			// If UsedMem is empty
			else
			{

				// Create new BlockDesc in UsedMem
				m_pUsedMemHead		= static_cast<BlockDesc*>(pFree->m_pBlockBase);
				pUsedNew			= m_pUsedMemHead;
				pUsedNew->m_pPrev	= nullptr;

			}


			// Assign new BlockDesc values in UsedMem
			pUsedNew->m_pBlockBase	= static_cast<void*>(pUsedNew + 1);
			pUsedNew->m_sizeBlock	= bytesToAlloc - sizeof(BlockDesc);
			pUsedNew->m_pNext		= nullptr;


			// Adjust BlocDesc values in FreeMem accordingly
			uintptr_t* uip_pFreeBlockBase	= static_cast<uintptr_t*>	(pFree->m_pBlockBase) + bytesToAlloc;
			void* v_pFreeBlockBase			= static_cast<void*>		(uip_pFreeBlockBase);
			pFree->m_pBlockBase				= v_pFreeBlockBase;
			pFree->m_sizeBlock				-= bytesToAlloc;


			// Assign return pointer and adjust for guard banding
			char*	c_pBlockBase	= static_cast<char*>	(pUsedNew->m_pBlockBase);
			char*	c_pReturn		= c_pBlockBase + GUARD_BANDING;
			pReturn					= static_cast<void*>	(c_pReturn);

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

	// If attempting to free a nullptr, return true
	if (!i_ptr)
	{
		return true;
	}

	
	// Get the block descriptor of the memory to be freed
	char*		c_i_ptr				= static_cast<char*>		(i_ptr);
	char*		c_pUsedBlockBase	= c_i_ptr - GUARD_BANDING;
	char*		c_pUsed				= c_pUsedBlockBase - sizeof(BlockDesc);
	void*		v_pUsed				= static_cast<void*>		(c_pUsed);
	BlockDesc*	pUsed				= static_cast<BlockDesc*>	(v_pUsed);


	// Remove block from Used Mem list
	//		If the BlocDesc is the head of UsedMem
	if (pUsed->m_pPrev == nullptr)
	{
		// If it's the only BlocDesc in UsedMem, set head to nullptr
		if (pUsed->m_pNext == nullptr)
		{

			m_pUsedMemHead = nullptr;

		}

		// Otherwise remove the head
		else
		{

			m_pUsedMemHead			= pUsed->m_pNext;
			pUsed->m_pNext->m_pPrev = nullptr;

		}
	}

	//		If the BlocDesc is the tail of UsedMem
	else if (pUsed->m_pNext == nullptr)
	{

		pUsed->m_pPrev->m_pNext = nullptr;

	}

	//		If the BlocDesc is the in middle of UsedMem
	else
	{

		pUsed->m_pPrev->m_pNext = pUsed->m_pNext;
		pUsed->m_pNext->m_pPrev = pUsed->m_pPrev;

	}
	

	BlockDesc* pFree = m_pFreeMemHead;
	
	// If FreeMem list is empty, insert the used block into the head of FreeMem & return true
	if (pFree == nullptr)
	{
		m_pFreeMemHead			= pUsed;
		m_pFreeMemHead->m_pPrev = nullptr;
		m_pFreeMemHead->m_pNext = nullptr;
		return true;
	}

	// Otherwise iterate through until our FreeMem block base is greater than our used memory block base
	while (pUsed->m_pBlockBase > pFree->m_pBlockBase)
	{
		pFree = pFree->m_pNext;
	}


	// Insert the used memory into FreeMem behind the block to which we iterated
	if (pFree != nullptr)
	{

		// If inserting into the start of FreeMem
		if (pFree->m_pPrev == nullptr)
		{

			pFree->m_pPrev = pUsed;
			pUsed->m_pPrev = nullptr;
			pUsed->m_pNext = pFree;
			m_pFreeMemHead = pUsed;

		}

		// If insterting anywhere else;
		else
		{

			pFree->m_pPrev->m_pNext		= pUsed;
			pUsed->m_pPrev				= pFree->m_pPrev;
			pFree->m_pPrev				= pUsed;
			pUsed->m_pNext				= pFree;

		}
		
		return true;
	}
	
	else
	{
		return false;
	}
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

// Displays the current free blocks and their relevant values
void HeapManager::ShowFreeBlocks()
{
	std::cout << "----- FREE BLOCKS -----" << std::endl;

	int blockCount = 1;
	BlockDesc* pFree = m_pFreeMemHead;
	while (pFree != nullptr)
	{
		std::cout << "Block "		<< blockCount			<< ":\t"		<< pFree	<< std::endl;
		std::cout << "\tBase:\t"	<< pFree->m_pBlockBase	<< std::endl;
		std::cout << "\tSize:\t"	<< pFree->m_sizeBlock	<< std::endl;
		std::cout << "\tNext:\t"	<< pFree->m_pNext		<< std::endl;
		blockCount++;
		pFree = pFree->m_pNext;
	}
	std::cout << std::endl;
}

// Displays the current used blocks and their relevant values
void HeapManager::ShowOutstandingAllocations()
{
	std::cout << "----- OUTSTANDING BLOCKS -----" << std::endl;

	int blockCount = 1;
	BlockDesc* pUsed = m_pUsedMemHead;
	while (pUsed != nullptr)
	{
		std::cout << "Block "		<< blockCount			<< ":\t"		<< pUsed	<< std::endl;
		std::cout << "\tBase:\t"	<< pUsed->m_pBlockBase	<< std::endl;
		std::cout << "\tSize:\t"	<< pUsed->m_sizeBlock	<< std::endl;
		std::cout << "\tNext:\t"	<< pUsed->m_pNext		<< std::endl;
		blockCount++;
		pUsed = pUsed->m_pNext;
	}
	std::cout << std::endl;
}

void HeapManager::destroy()
{
	return; //TODO: implement destroy
}