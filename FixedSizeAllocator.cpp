#include "FixedSizeAllocator.h"
#include "BitArray.h"
#include <stdint.h>

FixedSizeAllocator::FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize)
{

}

FixedSizeAllocator* FixedSizeAllocator::Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize)
{
	return nullptr;
}

FixedSizeAllocator::~FixedSizeAllocator()
{
	size_t firstClear;

	if (m_pFreeBits->FindFirstClearBit(firstClear))
	{
		// Outstanding allocation
	}

}

void* FixedSizeAllocator::_alloc()
{

	size_t firstAvailable;
	if (m_pFreeBits->FindFirstSetBit(firstAvailable))
	{

		m_pFreeBits->ClearBit(firstAvailable);
		
		uintptr_t uipFreeMem = reinterpret_cast<uintptr_t>(m_pFreeMem);
		uintptr_t uipUserPtr = uipFreeMem + (firstAvailable * m_BlockSize);
		void* pUserPtr = reinterpret_cast<void*>(uipUserPtr);

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
	// And that it's an outstanding allocation

	// Set bit for that block

	return true;
}