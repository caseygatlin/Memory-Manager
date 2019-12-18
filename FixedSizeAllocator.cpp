#include "FixedSizeAllocator.h"

FixedSizeAllocator::FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize)
{

}

FixedSizeAllocator* FixedSizeAllocator::Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize)
{
	return nullptr;
}


bool FixedSizeAllocator::_free(void* i_ptr)
{
	// Check that pointer is a valid pointer within this allocator
	// And that it's an outstanding allocation

	// Set bit for that block

	return true;
}