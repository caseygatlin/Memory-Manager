#include "DynamicMemory.h"
#include "FixedSizeAllocator.h"
#include "HeapManagerProxy.h"
#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>


class   HeapManager;
class   FixedSizeAllocator;


// External variables that hold the memory managers
extern  HeapManager*         S_DEFAULT_HEAP_MANAGER;
extern  FixedSizeAllocator*  S_FIXED_SIZE_ALLOCATORS[3];
extern  size_t               S_NUM_FIXED_SIZE_ALLOCATORS;



// Allocate memory using FSA or HeapManager
#pragma warning ( suppress : 28251 ) //Warning for inconsistent SAL Annotation for Free
void * __cdecl malloc(size_t i_size)
#pragma warning ( suppress : 4565 ) //Warnings for multiple definitions of malloc
{

    // Attempt to find an FSA to get memory
	void* pReturn                             = nullptr;
	FixedSizeAllocator* pFixedSizeAllocator   = FindFixedSizeAllocator(i_size);


    // If there's an FSA of a good size, attempt to allocate
	if (pFixedSizeAllocator)
	{

        // Allocate
		pReturn = pFixedSizeAllocator->_alloc();

	}


    // If no FSA or FSA can't alloc, use HeapManager
	if (pReturn == nullptr)
	{

		pReturn = HeapManagerProxy::alloc(S_DEFAULT_HEAP_MANAGER, i_size);

	}


    // Print & return
	printf("malloc %zu\n", i_size);
	return pReturn;

}




// Free memory from FSA or HeapManagr
#pragma warning ( suppress : 28251 ) //Warning for inconsistent SAL Annotation for Free
void __cdecl free(void * i_ptr)
#pragma warning ( suppress : 4565 ) //Warning for multiple definitions of Free
{

	if (i_ptr)
	{

        // Check if the memory is from an FSA
		for (size_t i = 0; i < S_NUM_FIXED_SIZE_ALLOCATORS; i++)
		{

			if (S_FIXED_SIZE_ALLOCATORS[i] && S_FIXED_SIZE_ALLOCATORS[i]->Contains(i_ptr))
			{

				S_FIXED_SIZE_ALLOCATORS[i]->_free(i_ptr);

				printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
				return;

			}
		}


        // If it's not from an FSA free it using the HeapManager
		if (S_DEFAULT_HEAP_MANAGER)
		{

			HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, i_ptr);

		}
	}


	printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
}



// Construct object and allocate memory using FSA or HeapManager
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



// Destruct object and free memory using FSA or HeapManager
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



// Construct and allocate an array of objects using FSA or HeapManager
void * operator new[](size_t i_size)
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


	printf("new [] %zu\n", i_size);
	return pReturn;

}



// Destruct and free an array of objects using FSA or HeapManager
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

}