#include <assert.h>
#include "HeapManager.h"
#include "BlockDesc.h"
#include <iostream>
#include <stdint.h>

// 4 byte guard bands in case of writing over allocated block
const size_t GUARD_BANDING = 4;
const uint8_t DEFAULT_ALIGNMENT = 4;


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
	// Adjust size to include Guard Banding & BlockDesc
	size_t	bytesToAlloc	= i_bytes;
			bytesToAlloc	+= sizeof(BlockDesc);
			bytesToAlloc	+= 2 * GUARD_BANDING;


	// Initialize return value
	void*	pReturn			= nullptr;


	// Search for big enough block,
	// create new block in UsedMem when found
	BlockDesc* pFree = m_pFreeMemHead;
	while (pFree != nullptr)
	{
		// Calculate alignment padding
		char*		c_pFreeBlockBase		= static_cast<char*>			(pFree->m_pBlockBase);
		char*		c_pFreeEnd				= c_pFreeBlockBase + pFree->m_sizeBlock;
		char*		c_pPotentialUsedStart	= c_pFreeEnd - bytesToAlloc;
		char*		c_pPotentialUsedUserPtr = c_pPotentialUsedStart + sizeof(BlockDesc) + GUARD_BANDING;

		uintptr_t	uip_userPtr				= reinterpret_cast<uintptr_t>	(c_pPotentialUsedUserPtr);
		int			padding = 0;


		while (uip_userPtr % DEFAULT_ALIGNMENT != 0)
		{

			uip_userPtr--;
			padding++;

		}

		// Add padding to allocation bytes
		bytesToAlloc += padding;


		// If current block is big enough, allocate memory
		if (pFree->m_sizeBlock >= bytesToAlloc)
		{

			BlockDesc* pUsed	= m_pUsedMemHead;
			BlockDesc* pUsedNew = nullptr;

			// Calculate starting position of block
			char* c_pUsedStart	= c_pFreeEnd - bytesToAlloc;
			void* v_pUsedStart	= static_cast<void*>(c_pUsedStart);

			// If UsedMem has at least one item
			if (pUsed != nullptr)
			{

				// Iterate till 2nd to last item
				while (pUsed->m_pNext != nullptr)
				{

					pUsed = pUsed->m_pNext;

				}

				// Create new BlockDesc in UsedMem
				pUsed->m_pNext		= static_cast<BlockDesc*>(v_pUsedStart);
				pUsedNew			= pUsed->m_pNext;
				pUsedNew->m_pPrev	= pUsed;

			}

			// If UsedMem is empty
			else
			{

				// Create new BlockDesc in UsedMem
				m_pUsedMemHead		= static_cast<BlockDesc*>(v_pUsedStart);
				pUsedNew			= m_pUsedMemHead;
				pUsedNew->m_pPrev	= nullptr;

			}


			// Assign new BlockDesc values
			pUsedNew->m_pBlockBase	= static_cast<void*>(pUsedNew + 1);
			pUsedNew->m_sizeBlock	= bytesToAlloc - sizeof(BlockDesc);
			pUsedNew->m_pNext		= nullptr;


			// Adjust BlocDesc size in FreeMem accordingly
			pFree->m_sizeBlock -= bytesToAlloc;


			// Assign return pointer
			char*	c_pReturn	= static_cast<char*>	(pUsedNew->m_pBlockBase);
					c_pReturn	+= GUARD_BANDING;
					pReturn		= static_cast<void*>	(c_pReturn);

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
	// Adjust size to include Guard Banding & BlockDesc
	size_t	bytesToAlloc	= i_bytes;
			bytesToAlloc	+= sizeof(BlockDesc);
			bytesToAlloc	+= 2 * GUARD_BANDING;


	// Initialize return value
	void*	pReturn			= nullptr;


	// Search for big enough block,
	// create new block in UsedMem when found
	BlockDesc* pFree = m_pFreeMemHead;
	while (pFree != nullptr)
	{
		// Calculate alignment padding
		char*		c_pFreeBlockBase		= static_cast<char*>			(pFree->m_pBlockBase);
		char*		c_pFreeEnd				= c_pFreeBlockBase + pFree->m_sizeBlock;
		char*		c_pPotentialUsedStart	= c_pFreeEnd - bytesToAlloc;
		char*		c_pPotentialUsedUserPtr	= c_pPotentialUsedStart + sizeof(BlockDesc) + GUARD_BANDING;
		
		uintptr_t	uip_userPtr				= reinterpret_cast<uintptr_t>	(c_pPotentialUsedUserPtr);
		int			padding					= 0;


		while (uip_userPtr % i_alignment != 0)
		{

			uip_userPtr--;
			padding++;

		}

		// Add padding to allocation bytes
		bytesToAlloc += padding;

		// If current block is big enough, allocate memory
		if (pFree->m_sizeBlock >= bytesToAlloc)
		{
			BlockDesc* pUsed	= m_pUsedMemHead;
			BlockDesc* pUsedNew = nullptr;

			// Calculate starting position of block
			char* c_pUsedStart	= c_pFreeEnd - bytesToAlloc;
			void* v_pUsedStart	= static_cast<void*>(c_pUsedStart);

			// If UsedMem has at least one item
			if (pUsed != nullptr)
			{

				// Iterate till 2nd to last item
				while (pUsed->m_pNext != nullptr)
				{

					pUsed = pUsed->m_pNext;

				}

				// Create new BlockDesc in UsedMem
				pUsed->m_pNext		= static_cast<BlockDesc*>(v_pUsedStart);
				pUsedNew			= pUsed->m_pNext;
				pUsedNew->m_pPrev	= pUsed;

			}

			// If UsedMem is empty
			else
			{

				// Create new BlockDesc in UsedMem
				m_pUsedMemHead		= static_cast<BlockDesc*>(v_pUsedStart);
				pUsedNew			= m_pUsedMemHead;
				pUsedNew->m_pPrev	= nullptr;

			}


			// Assign new BlockDesc values
			pUsedNew->m_pBlockBase	= static_cast<void*>(pUsedNew + 1);
			pUsedNew->m_sizeBlock	= bytesToAlloc - sizeof(BlockDesc);
			pUsedNew->m_pNext		= nullptr;


			// Adjust BlocDesc size in FreeMem accordingly
			pFree->m_sizeBlock -= bytesToAlloc;


			// Assign return pointer
			char*	c_pReturn	= static_cast<char*>	(pUsedNew->m_pBlockBase);
					c_pReturn	+= GUARD_BANDING;
					pReturn		= static_cast<void*>	(c_pReturn);

			//Exit loop and return
			return pReturn;

		}

		// Otherwise move to next block
		pFree = pFree->m_pNext;

	}

	// If no block big enough is found, return nullptr
	return nullptr;
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

	// Store a copy of previous iteration
	BlockDesc* pFreePrev = nullptr;

	// Otherwise iterate through until our FreeMem block base is greater than our used memory block base
	while (pFree != nullptr && pUsed->m_pBlockBase > pFree->m_pBlockBase)
	{
		pFreePrev = pFree;
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
	
	// Insert at the end, if doesn't fit inside the list
	else
	{
		pFreePrev->m_pNext	= pUsed;
		pUsed->m_pPrev		= pFreePrev;
		pUsed->m_pNext		= nullptr;

		return true;
	}
}

void HeapManager::collect()
{
	// Define iterator
	BlockDesc* pFree = m_pFreeMemHead;

	while (pFree->m_pNext != nullptr)
	{

		// Calculate end of pFree and the address of the next block
		char* c_pFreeBlockBase	= static_cast<char*>	(pFree->m_pBlockBase);
		char* c_pFreeEndBlock	= c_pFreeBlockBase + pFree->m_sizeBlock;
		void* v_pFreeEndBlock	= static_cast<void*>	(c_pFreeEndBlock);
		void* v_pFreeNext		= static_cast<void*>	(pFree->m_pNext);


		// If pFree can collect the next block
		if (v_pFreeNext == v_pFreeEndBlock)
		{

			// Resize collecting block
			pFree->m_sizeBlock += sizeof(BlockDesc) + pFree->m_pNext->m_sizeBlock;

			// Define block to be collected
			BlockDesc* pCollectedBlock = pFree->m_pNext;


			// If elements after pCollectedBlock
			if (pCollectedBlock->m_pNext != nullptr)
			{

				// Remove collected block from FreeMem
				pCollectedBlock->m_pNext->m_pPrev	= pFree;
				pFree->m_pNext						= pCollectedBlock->m_pNext;

				// Nullify collected block
				pCollectedBlock->m_pNext		= nullptr;
				pCollectedBlock->m_pPrev		= nullptr;
				pCollectedBlock->m_sizeBlock	= 0;
				pCollectedBlock->m_pBlockBase	= nullptr;
				pCollectedBlock					= nullptr;

			}

			// If no elements after pCollectedBlock
			else
			{

				// Remove collected block from FreeMem
				pFree->m_pNext = nullptr;

				// Nullify collected block
				pCollectedBlock->m_pNext		= nullptr;
				pCollectedBlock->m_pPrev		= nullptr;
				pCollectedBlock->m_sizeBlock	= 0;
				pCollectedBlock->m_pBlockBase	= nullptr;
				pCollectedBlock					= nullptr;

			}
		}


		// If no collecting can occur
		else
		{

			// Iterate to next
			pFree = pFree->m_pNext;

		}
	}
}

size_t HeapManager::getLargestFreeBlock()
{
	// Define max & iterator
	size_t		max		= 0;
	BlockDesc*	pFree	= m_pFreeMemHead;

	// Iterate to find max
	while (pFree != nullptr)
	{

		if (pFree->m_sizeBlock > max)
		{

			max = pFree->m_sizeBlock;

		}

		pFree = pFree->m_pNext;

	}

	return max;
}

size_t HeapManager::getTotalFreeMemory()
{
	// Define size & iterator
	size_t		total	= 0;
	BlockDesc*	pFree	= m_pFreeMemHead;

	// Iterate and accumulate total
	while (pFree != nullptr)
	{

		total += pFree->m_sizeBlock;
		pFree = pFree->m_pNext;

	}

	return total;
}

bool HeapManager::Contains(void* i_ptr)
{
	// Define iterator for FreeMem
	BlockDesc* pFree = m_pFreeMemHead;

	// Iterate and compare
	while (pFree != nullptr)
	{
		// Calculate iterated block user ptr
		char*	c_pFreeBlockBase	= static_cast<char*>	(pFree->m_pBlockBase);
		char*	c_pFreeUserPtr		= c_pFreeBlockBase + GUARD_BANDING;
		void*	v_pFreeUserPtr		= static_cast<void*>	(c_pFreeUserPtr);

		// Compare user ptr with given ptr
		if (v_pFreeUserPtr == i_ptr)
		{

			return true;

		}

		pFree = pFree->m_pNext;

	}

	// If not in FreeMem, check if it's allocated
	return IsAllocated(i_ptr);
}

bool HeapManager::IsAllocated(void* i_ptr)
{
	// Define iterator
	BlockDesc* pUsed = m_pUsedMemHead;

	// Iterate and compare
	while (pUsed != nullptr)
	{
		// Calculate iterated block user ptr
		char*	c_pUsedBlockBase	= static_cast<char*>	(pUsed->m_pBlockBase);
		char*	c_pUsedUserPtr		= c_pUsedBlockBase + GUARD_BANDING;
		void*	v_pUsedUserPtr		= static_cast<void*>	(c_pUsedUserPtr);
		
		// Compare user ptr with given ptr
		if (v_pUsedUserPtr == i_ptr)
		{

			return true;

		}

		pUsed = pUsed->m_pNext;

	}

	return false;
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

	// Free all outstanding memory
	while (m_pUsedMemHead != nullptr)
	{

		char* c_pUsedBlockBase	= static_cast<char*>	(m_pUsedMemHead->m_pBlockBase);
		char* c_pUsedUserPtr	= c_pUsedBlockBase + GUARD_BANDING;
		void* v_pUsedUserPtr	= static_cast<void*>	(c_pUsedUserPtr);

		_free(v_pUsedUserPtr);
	}

	// Collect into one block
	collect();

	// Nullify that block
	m_pFreeMemHead->m_pBlockBase	= nullptr;
	m_pFreeMemHead->m_pNext			= nullptr;
	m_pFreeMemHead->m_pPrev			= nullptr;
	m_pFreeMemHead->m_sizeBlock		= 0;
	m_pFreeMemHead					= nullptr;
}