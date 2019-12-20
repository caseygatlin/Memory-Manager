#pragma once

#include <stdint.h>

class HeapManager;

namespace HeapManagerProxy
{
	inline HeapManager *	CreateHeapManager( const void * i_pMemory, const size_t& i_sizeMemory);
	inline void				Destroy( HeapManager * i_pManager );

	inline void *			alloc( HeapManager * i_pManager, const size_t& i_size );
	inline void *			alloc( HeapManager * i_pManager, const size_t& i_size, const unsigned int& i_alignment );
	inline bool				free( HeapManager * i_pManager, void * i_ptr );

	inline void				Collect( HeapManager * i_pManager );

	inline bool				Contains( const HeapManager * i_pManager, const void * i_ptr );
	inline bool				IsAllocated( const HeapManager * i_pManager, const void * i_ptr );

	inline size_t			GetLargestFreeBlock( const HeapManager * i_pManager );
	inline size_t			GetTotalFreeMemory( const HeapManager * i_pManager );

	inline void				ShowFreeBlocks( const HeapManager * i_pManager );
	inline void				ShowOutstandingAllocations( const HeapManager * i_pManager );

} // namespace HeapManagerProxy

#include "HeapManagerProxy-inl.h"

