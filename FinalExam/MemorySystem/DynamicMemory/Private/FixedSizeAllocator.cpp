#include "BitArray.h"
#include "FixedSizeAllocator.h"
#include <stdint.h>
#include <stdio.h>



class   HeapManager;


// External variables that hold the memory managers
extern  HeapManager*         S_DEFAULT_HEAP_MANAGER;
extern  FixedSizeAllocator*  S_FIXED_SIZE_ALLOCATORS[3];
extern  size_t               S_NUM_FIXED_SIZE_ALLOCATORS;



// Creates and constructs an FSA
FixedSizeAllocator* FixedSizeAllocator::Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize)
{

	// Defines new FSA in memory
	FixedSizeAllocator* pFSA    = static_cast<FixedSizeAllocator*>(const_cast<void*>(i_pHeapMemory));


	// Constructs FSA
	uintptr_t   uip_FSAMemory   = reinterpret_cast<uintptr_t>  (i_pHeapMemory);
	void*       v_pFSAStart     = reinterpret_cast<void*>      (uip_FSAMemory + sizeof(FixedSizeAllocator));
			    *pFSA           = FixedSizeAllocator(v_pFSAStart, i_heapMemorySize, i_numBlocks, i_blockSize);


	return pFSA;

}



// Constructor
FixedSizeAllocator::FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize)
{

    // Defines member variables
    m_memorySize    = i_heapMemorySize;
    m_BlockSize     = i_blockSize;
    m_pFreeMem      = const_cast<void*>(i_pHeapMemory);


    // Calculates start of BitArray
    uintptr_t   uip_BitArrayStart = reinterpret_cast<uintptr_t>(m_pFreeMem);
                uip_BitArrayStart += i_numBlocks * i_blockSize;


    // Constructs BitArray at that location
    m_pFreeBits = BitArray::Create(i_numBlocks, reinterpret_cast<void*>(uip_BitArrayStart));

}



// Destructor
FixedSizeAllocator::~FixedSizeAllocator()
{

    // Check for outstanding allocations
	size_t firstClear;

	if (m_pFreeBits && m_pFreeBits->FindFirstClearBit(firstClear))
	{

#ifdef _DEBUG
		printf("\nFixed Size Allocator has outstanding memory...\n");
#endif

	}

    else
    {
        // Nullify member variables
        m_pFreeBits->   ~BitArray();
        m_memorySize = 0;
        m_BlockSize = 0;
        m_pFreeMem = nullptr;
        m_pFreeBits = nullptr;
    }
}



// Allocate memory
void* FixedSizeAllocator::_alloc()
{

    // Find a set bit and return the corresponding memory location
	size_t firstAvailable;
	if (m_pFreeBits->FindFirstSetBit(firstAvailable))
	{

		m_pFreeBits->ClearBit(firstAvailable);

		uintptr_t   uipFreeMem   = reinterpret_cast<uintptr_t>  (m_pFreeMem);
		uintptr_t   uipUserPtr   = uipFreeMem + (firstAvailable * m_BlockSize);
		void*       pUserPtr     = reinterpret_cast<void*>      (uipUserPtr);

		return pUserPtr;

	}


    // If no open memory, return nullptr
	else
	{

		return nullptr;

	}

}



// Free memory given a pointer
bool FixedSizeAllocator::_free(void* i_ptr)
{

	// Check that pointer is a valid pointer within this allocator
	if (i_ptr && Contains(i_ptr))
	{

        // Create int versions of member variable addresses
		uintptr_t uip_Ptr       = reinterpret_cast<uintptr_t>(i_ptr);
		uintptr_t uip_FreeMem   = reinterpret_cast<uintptr_t>(m_pFreeMem);
		uintptr_t uip_FreeBits  = reinterpret_cast<uintptr_t>(m_pFreeBits);


		// Check if it's an outstanding allocation
		size_t index = 0;
		while (uip_Ptr != uip_FreeMem + m_BlockSize * index && uip_FreeMem + m_BlockSize * index < uip_FreeBits)
		{

			index++;

		}

        // If the ptr matches the memory location it should, set bit and return true
		if (uip_Ptr == uip_FreeMem + m_BlockSize * index)
		{

			m_pFreeBits->SetBit(index);
			return true;

		}
		
	}


    // If the memory didn't match with a location, return false
	return false;

}



// Check whether a memory address is contained in this FSA
bool FixedSizeAllocator::Contains(void* i_ptr) const
{

	if (i_ptr)
	{

        //Check whether the given pointer is within the correct range of addresses
		uintptr_t uip_Ptr       = reinterpret_cast<uintptr_t>(i_ptr);
		uintptr_t uip_FreeMem   = reinterpret_cast<uintptr_t>(m_pFreeMem);


		if (uip_Ptr >= uip_FreeMem && uip_Ptr < uip_FreeMem + m_memorySize)
		{

			return true;

		}

	}


    // If not a valid ptr or not in range, return false
	return false;

}



// Return the size blocks this FSA manages
size_t FixedSizeAllocator::GetBlockSize() const
{

	return m_BlockSize;

}

