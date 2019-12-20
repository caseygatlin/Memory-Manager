#include "BitArray.h"
#include "FixedSizeAllocator.h"
#include <stdint.h>
#include <stdio.h>

class HeapManager;

extern HeapManager* S_DEFAULT_HEAP_MANAGER;
extern FixedSizeAllocator* S_FIXED_SIZE_ALLOCATORS[5];
extern size_t S_NUM_FIXED_SIZE_ALLOCATORS;


FixedSizeAllocator::FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize)
{
	m_memorySize  = i_heapMemorySize;
	m_BlockSize   = i_blockSize;
	m_pFreeMem    = const_cast<void*>(i_pHeapMemory);
			

	uintptr_t   uip_BitArrayStart   = reinterpret_cast<uintptr_t>(m_pFreeMem);
			    uip_BitArrayStart   += i_numBlocks * i_blockSize;


	m_pFreeBits = BitArray::Create(i_numBlocks, reinterpret_cast<void*>(uip_BitArrayStart));

}



FixedSizeAllocator* FixedSizeAllocator::Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize)
{

	// Defines new FSA in memory
	FixedSizeAllocator* pFSA = static_cast<FixedSizeAllocator*>(const_cast<void*>(i_pHeapMemory));


	// Constructs FSA
	uintptr_t   uip_FSAMemory   = reinterpret_cast<uintptr_t>  (i_pHeapMemory);
	void*       v_pFSAStart     = reinterpret_cast<void*>      (uip_FSAMemory + sizeof(FixedSizeAllocator));
			    *pFSA           = FixedSizeAllocator(v_pFSAStart, i_heapMemorySize, i_numBlocks, i_blockSize);


	return pFSA;

}



FixedSizeAllocator::~FixedSizeAllocator()
{

	size_t firstClear;

	if (m_pFreeBits->FindFirstClearBit(firstClear))
	{

		printf("Fixed Size Allocator has outstanding memory...");

	}
	else
	{
		m_pFreeBits->~BitArray();
		m_memorySize = 0;
		m_BlockSize = 0;
		m_pFreeMem = nullptr;
		m_pFreeBits = nullptr;
	}

}



void* FixedSizeAllocator::_alloc()
{

	size_t firstAvailable;
	if (m_pFreeBits->FindFirstSetBit(firstAvailable))
	{

		m_pFreeBits->ClearBit(firstAvailable);

		uintptr_t   uipFreeMem   = reinterpret_cast<uintptr_t>  (m_pFreeMem);
		uintptr_t   uipUserPtr   = uipFreeMem + (firstAvailable * m_BlockSize);
		void*       pUserPtr     = reinterpret_cast<void*>      (uipUserPtr);

		return pUserPtr;

	}

	else
	{

		return nullptr;

	}

}



bool FixedSizeAllocator::_free(void* i_ptr)
{

	// Check that pointer is a valid pointer within this allocator
	if (i_ptr && Contains(i_ptr))
	{
		uintptr_t uip_Ptr = reinterpret_cast<uintptr_t>(i_ptr);
		uintptr_t uip_FreeMem = reinterpret_cast<uintptr_t>(m_pFreeMem);
		uintptr_t uip_FreeBits = reinterpret_cast<uintptr_t>(m_pFreeBits);

		// Check if it's an outstanding allocation
		size_t index = 0;
		while (uip_Ptr != uip_FreeMem + m_BlockSize * index && uip_FreeMem + m_BlockSize * index < uip_FreeBits)
		{
			index++;
		}

		if (uip_Ptr == uip_FreeMem + m_BlockSize * index)
		{
			m_pFreeBits->SetBit(index);
			return true;
		}
		
	}

	return false;

}

bool FixedSizeAllocator::Contains(void* i_ptr) const
{
	if (i_ptr)
	{
		uintptr_t uip_Ptr = reinterpret_cast<uintptr_t>(i_ptr);
		uintptr_t uip_FreeMem = reinterpret_cast<uintptr_t>(m_pFreeMem);
		if (uip_Ptr >= uip_FreeMem && uip_Ptr < uip_FreeMem + m_memorySize)
		{
			return true;
		}
	}

	return false;
}


size_t FixedSizeAllocator::GetBlockSize() const
{

	return m_BlockSize;

}

