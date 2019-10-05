#pragma once


// Block descriptor used for keeping track of allocated blocks of memory
struct BlockDesc
{
	void*			m_pBlockBase;	// The allocated memory address in the heap manager
	size_t			m_sizeBlock;	// The size of this block
	BlockDesc*		m_pNext;		// Pointer to the next block
};