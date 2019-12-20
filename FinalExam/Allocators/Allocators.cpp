#include "FixedSizeAllocator.h"
#include "DynamicMemory.h"
#include "HeapManagerProxy.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>

class HeapManager;
class FixedSizeAllocator;

extern HeapManager* S_DEFAULT_HEAP_MANAGER;
extern FixedSizeAllocator* S_FIXED_SIZE_ALLOCATORS[5];
extern size_t S_NUM_FIXED_SIZE_ALLOCATORS;

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

		pReturn = HeapManagerProxy::alloc(S_DEFAULT_HEAP_MANAGER, i_size);

	}


	printf("malloc %zu\n", i_size);
	return pReturn;

}

void __cdecl free(void * i_ptr)
{
	if (i_ptr)
	{
		for (size_t i = 0; i < S_NUM_FIXED_SIZE_ALLOCATORS; i++)
		{
			if (S_FIXED_SIZE_ALLOCATORS[i]->Contains(i_ptr))
			{
				S_FIXED_SIZE_ALLOCATORS[i]->_free(i_ptr);

				printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
				return;
			}
		}

		if (S_DEFAULT_HEAP_MANAGER)
		{
			HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, i_ptr);
		}
	}

	printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
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

		pReturn = HeapManagerProxy::alloc(GetDefaultHeapManager(), i_size);

	}


	printf("new %zu\n", i_size);
	return pReturn;

}

void operator delete(void * i_ptr)
{
	if (i_ptr)
	{
		for (size_t i = 0; i < S_NUM_FIXED_SIZE_ALLOCATORS; i++)
		{
			if (S_FIXED_SIZE_ALLOCATORS[i]->Contains(i_ptr))
			{
				S_FIXED_SIZE_ALLOCATORS[i]->_free(i_ptr);

				printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
				return;
			}
		}

		if (S_DEFAULT_HEAP_MANAGER)
		{
			HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, i_ptr);
		}
	}

	printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
}

void * operator new[](size_t i_size)
{
	void* pReturn = nullptr;
	FixedSizeAllocator* pFixedSizeAllocator = FindFixedSizeAllocator(i_size);

	if (pFixedSizeAllocator)
	{

		pReturn = pFixedSizeAllocator->_alloc();

	}


	if (pReturn == nullptr)
	{

		pReturn = HeapManagerProxy::alloc(S_DEFAULT_HEAP_MANAGER, i_size);

	}

	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new [] %zu\n", i_size);
	return _aligned_malloc(i_size, 4);

}

void operator delete [](void * i_ptr)
{
	if (i_ptr)
	{
		for (size_t i = 0; i < S_NUM_FIXED_SIZE_ALLOCATORS; i++)
		{
			if (S_FIXED_SIZE_ALLOCATORS[i]->Contains(i_ptr))
			{
				S_FIXED_SIZE_ALLOCATORS[i]->_free(i_ptr);

				printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
				return;
			}
		}

		if (S_DEFAULT_HEAP_MANAGER)
		{
			HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, i_ptr);
		}
	}


	printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return _aligned_free(i_ptr);

}