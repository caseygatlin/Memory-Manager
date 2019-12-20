#include "FixedSizeAllocator.h"
#include "DynamicMemory.h"
#include "HeapManager.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>

void * __cdecl malloc(size_t i_size)
{

	void* pReturn                             = nullptr;
	FixedSizeAllocator* pFixedSizeAllocator   = FindFixedSizeAllocator(i_size);

	if (pFixedSizeAllocator)
	{

		pReturn = pFixedSizeAllocator->_alloc();

	}


	if (pReturn == nullptr)
	{

		pReturn = S_DEFAULT_HEAP_MANAGER->_alloc(i_size);

	}


	printf("malloc %zu\n", i_size);
	return pReturn;

}

void __cdecl free(void * i_ptr)
{

	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return _aligned_free(i_ptr);

}

void * operator new(size_t i_size)
{

	void* pReturn                             = nullptr;
	FixedSizeAllocator* pFixedSizeAllocator   = FindFixedSizeAllocator(i_size);

	if (pFixedSizeAllocator)
	{

		pReturn = pFixedSizeAllocator->_alloc();

	}


	if (pReturn == nullptr)
	{

		pReturn = S_DEFAULT_HEAP_MANAGER->_alloc(i_size);

	}


	printf("malloc %zu\n", i_size);
	return pReturn;

}

void operator delete(void * i_ptr)
{

	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return _aligned_free(i_ptr);

}

void * operator new[](size_t i_size)
{

	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new [] %zu\n", i_size);
	return _aligned_malloc(i_size, 4);

}

void operator delete [](void * i_ptr)
{

	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return _aligned_free(i_ptr);

}