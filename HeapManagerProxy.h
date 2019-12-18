#ifndef __HEAP_MANAGER_PROXY_H
#define __HEAP_MANAGER_PROXY_H

#include <stdint.h>

class HeapManager;
class FixedSizeAllocator;

namespace HeapManagerProxy
{
	inline HeapManager *	   CreateHeapManager( const void * i_pMemory, const size_t& i_sizeMemory );
	inline void				   Destroy( HeapManager * i_pManager );

	inline void				   CreateFixedSizeAllocator( const size_t& i_sizeBlock, const size_t& i_numBlocks, HeapManager* i_pManager );
	inline FixedSizeAllocator* GetFixedAllocator(const size_t& i_sizeBlock, HeapManager* i_pHeapManager);

	inline void *			   alloc( HeapManager * i_pManager, const size_t& i_size );
	inline void *			   alloc( HeapManager * i_pManager, const size_t& i_size, const unsigned int& i_alignment );
	inline bool				   free( HeapManager * i_pManager, void * i_ptr );

	inline void				   Collect( HeapManager * i_pManager );

	inline bool				   Contains( const HeapManager * i_pManager, const void * i_ptr );
	inline bool				   IsAllocated( const HeapManager * i_pManager, const void * i_ptr );

	inline size_t			   GetLargestFreeBlock( const HeapManager * i_pManager );
	inline size_t			   GetTotalFreeMemory( const HeapManager * i_pManager );

	inline void				   ShowFreeBlocks( const HeapManager * i_pManager );
	inline void				   ShowOutstandingAllocations( const HeapManager * i_pManager );

} // namespace HeapManagerProxy

#include "HeapManagerProxy-inl.h"

#endif // __HEAP_MANAGER_PROXY_H