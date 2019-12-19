#pragma once

#include <stdint.h>

namespace memory_system
{
	namespace dynamic_memory
	{

		// Block descriptor used for keeping track of allocated blocks of memory
		struct BlockDesc
		{
			void*		m_pBlockBase;	// The allocated memory address in the heap manager
			BlockDesc*	m_pNext;		// Pointer to the next block
			BlockDesc*	m_pPrev;		// Pointer to the previous block
			size_t		m_sizeBlock;	// The size of this block
		};
	}
}
