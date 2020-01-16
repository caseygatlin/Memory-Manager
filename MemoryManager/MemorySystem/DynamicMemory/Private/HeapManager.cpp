#include "BlockDesc.h"
#include "HeapManager.h"
#include <assert.h>
#include <iostream>
#include <stdint.h>



// 4 byte guard bands in case of writing over allocated block
const uint8_t GUARD_BANDING     = 4;
const uint8_t DEFAULT_ALIGNMENT = 4;


// Inizializes new HeapManager
HeapManager* HeapManager::create(const void* i_pHeapMemory, const size_t& i_heapMemorySize)
{
	// Defines new HeapManager in memory
	HeapManager*	pManager		= static_cast<HeapManager*>		(const_cast<void*>(i_pHeapMemory));


	// Constructs heap manager
	uintptr_t		uip_pHeapMemory	= reinterpret_cast<uintptr_t>	(i_pHeapMemory);
	void*			v_pHeapStart	= reinterpret_cast<void*>		(uip_pHeapMemory + sizeof(HeapManager));
					*pManager		= HeapManager(v_pHeapStart, i_heapMemorySize);

	return pManager;
}



// Constructor
HeapManager::HeapManager(const void* i_pHeapMemory, const size_t& i_heapMemorySize)
{

    // Calculate start of heap memory
    uintptr_t		uip_pHeapMemory = reinterpret_cast<uintptr_t>	(i_pHeapMemory);
    void*           v_pHeapStart    = reinterpret_cast<void*>		(uip_pHeapMemory + sizeof(BlockDesc));

    // Define FreeMem
    m_pFreeMemHead                  = static_cast<BlockDesc*>(const_cast<void*>(i_pHeapMemory));
    m_pFreeMemHead->m_pBlockBase    = v_pHeapStart;
    m_pFreeMemHead->m_pPrev         = nullptr;
    m_pFreeMemHead->m_pNext         = nullptr;
    m_pFreeMemHead->m_sizeBlock     = i_heapMemorySize - sizeof(BlockDesc);

    // Define empty UsedMem
    m_pUsedMemHead = nullptr;

}



// Allocates a given number of bytes and returns memory address
void* HeapManager::_alloc(const size_t& i_bytes)
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
		uintptr_t	uip_pFreeBlockBase			= reinterpret_cast<uintptr_t>			(pFree->m_pBlockBase);
		uintptr_t	uip_pFreeEnd				= uip_pFreeBlockBase + pFree->m_sizeBlock;
		uintptr_t	uip_pPotentialUsedStart		= uip_pFreeEnd - bytesToAlloc;
		uintptr_t	uip_pPotentialUsedUserPtr	= uip_pPotentialUsedStart + sizeof(BlockDesc) + GUARD_BANDING;

		uintptr_t	uip_userPtr					= uip_pPotentialUsedUserPtr;
		size_t		padding = 0;


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
			uintptr_t	uip_pUsedStart	= uip_pFreeEnd - bytesToAlloc;
			void*		v_pUsedStart	= reinterpret_cast<void*>(uip_pUsedStart);

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
			uintptr_t	uip_pReturn	= reinterpret_cast<uintptr_t>	(pUsedNew->m_pBlockBase);
						uip_pReturn	+= GUARD_BANDING;
						pReturn		= reinterpret_cast<void*>		(uip_pReturn);

			//Exit loop and return
			return pReturn;

		}

		// Otherwise move to next block
		pFree = pFree->m_pNext;

	}

	// If no block big enough is found, return nullptr
	return nullptr;
}



// Allocates given number of bytes and returns address on given alignment
void* HeapManager::_alloc(const size_t& i_bytes, const unsigned int& i_alignment)
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
		uintptr_t		uip_pFreeBlockBase			= reinterpret_cast<uintptr_t>(pFree->m_pBlockBase);
		uintptr_t		uip_pFreeEnd				= uip_pFreeBlockBase + pFree->m_sizeBlock;
		uintptr_t		uip_pPotentialUsedStart		= uip_pFreeEnd - bytesToAlloc;
		uintptr_t		uip_pPotentialUsedUserPtr	= uip_pPotentialUsedStart + sizeof(BlockDesc) + GUARD_BANDING;
		
		uintptr_t	uip_userPtr				= uip_pPotentialUsedUserPtr;
		size_t		padding					= 0;


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
			uintptr_t	uip_pUsedStart	= uip_pFreeEnd - bytesToAlloc;
			void*		v_pUsedStart	= reinterpret_cast<void*>(uip_pUsedStart);

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
			uintptr_t	uip_pReturn	= reinterpret_cast<uintptr_t>	(pUsedNew->m_pBlockBase);
					    uip_pReturn	+= GUARD_BANDING;
						pReturn		= reinterpret_cast<void*>		(uip_pReturn);

			//Exit loop and return
			return pReturn;

		}

		// Otherwise move to next block
		pFree = pFree->m_pNext;

	}

	// If no block big enough is found, return nullptr
	return nullptr;
}



// Frees a given memory address
bool HeapManager::_free(void* i_ptr)
{

	// If attempting to free a nullptr, return true
	if (!i_ptr)
	{
		return true;
	}

	
	// Get the block descriptor of the memory to be freed
	uintptr_t	uip_i_ptr			= reinterpret_cast<uintptr_t>	(i_ptr);
	uintptr_t	uip_pUsedBlockBase	= uip_i_ptr - GUARD_BANDING;
	uintptr_t	uip_pUsed			= uip_pUsedBlockBase - sizeof(BlockDesc);
	void*		v_pUsed				= reinterpret_cast<void*>		(uip_pUsed);
	BlockDesc*	pUsed				= static_cast<BlockDesc*>		(v_pUsed);


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



// Coalesces free blocks adjacent in memory
void HeapManager::collect()
{

	// Define iterator
	BlockDesc* pFree = m_pFreeMemHead;

	while (pFree->m_pNext != nullptr)
	{

		// Calculate end of pFree and the address of the next block
		uintptr_t	uip_pFreeBlockBase	= reinterpret_cast<uintptr_t>	(pFree->m_pBlockBase);
		uintptr_t	uip_pFreeEndBlock	= uip_pFreeBlockBase + pFree->m_sizeBlock;
		void*		v_pFreeEndBlock		= reinterpret_cast<void*>	    (uip_pFreeEndBlock);
		void*		v_pFreeNext			= static_cast<void*>	        (pFree->m_pNext);


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



// Returns the size of the largest free block
size_t HeapManager::getLargestFreeBlock() const
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



// Returns the total amount of free memory
size_t HeapManager::getTotalFreeMemory() const
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



// Check whether a memory address is contained in this manager
bool HeapManager::Contains(const void* i_ptr) const
{

	// Define iterator for FreeMem
	BlockDesc* pFree = m_pFreeMemHead;

	// Iterate and compare
	while (pFree != nullptr)
	{
		// Calculate iterated block user ptr
		uintptr_t	uip_pFreeBlockBase	= reinterpret_cast<uintptr_t>	(pFree->m_pBlockBase);
		uintptr_t	uip_pFreeUserPtr	= uip_pFreeBlockBase + GUARD_BANDING;
		void*		v_pFreeUserPtr		= reinterpret_cast<void*>		(uip_pFreeUserPtr);

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



// Check whether a memory address has been allocated
bool HeapManager::IsAllocated(const void* i_ptr) const
{

	// Define iterator
	BlockDesc* pUsed = m_pUsedMemHead;

	// Iterate and compare
	while (pUsed != nullptr)
	{
		// Calculate iterated block user ptr
		uintptr_t	uip_pUsedBlockBase	= reinterpret_cast<uintptr_t>	(pUsed->m_pBlockBase);
		uintptr_t	uip_pUsedUserPtr	= uip_pUsedBlockBase + GUARD_BANDING;
		void*		v_pUsedUserPtr		= reinterpret_cast<void*>		(uip_pUsedUserPtr);
		
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
void HeapManager::ShowFreeBlocks() const
{
	std::cout << "----- FREE BLOCKS -----" << std::endl;

	size_t blockCount = 1;
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
void HeapManager::ShowOutstandingAllocations() const
{
	std::cout << "----- OUTSTANDING BLOCKS -----" << std::endl;

	size_t blockCount = 1;
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



// Frees and collects memory, nullifies HeapManager
void HeapManager::destroy()
{

	// Free all outstanding memory
	while (m_pUsedMemHead != nullptr)
	{

		uintptr_t	uip_pUsedBlockBase	= reinterpret_cast<uintptr_t>	(m_pUsedMemHead->m_pBlockBase);
		uintptr_t	uip_pUsedUserPtr	= uip_pUsedBlockBase + GUARD_BANDING;
		void*		v_pUsedUserPtr		= reinterpret_cast<void*>		(uip_pUsedUserPtr);

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