#include "Allocators.h"
#include "HeapManagerProxy.h"
#include <Windows.h>
#include <assert.h>

void HeapManagerInit()
{
	if (S_DEFAULT_HEAP_MANAGER == nullptr)
	{
		using namespace HeapManagerProxy;
		const size_t sizeHeap = 1024 * 1024;

		void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
		assert(pHeapMemory);

		S_DEFAULT_HEAP_MANAGER = CreateHeapManager(pHeapMemory, sizeHeap);
		assert(S_DEFAULT_HEAP_MANAGER);
	}
}

void* operator new(size_t i_size)
{
	if (S_DEFAULT_HEAP_MANAGER == nullptr)
	{
		HeapManagerInit();
	}

	return HeapManagerProxy::alloc(S_DEFAULT_HEAP_MANAGER, i_size);
}


void operator delete(void* i_ptr)
{
	if (i_ptr && S_DEFAULT_HEAP_MANAGER)
	{
		HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, i_ptr);
	}
}

void* operator new[](size_t i_size)
{
	if (S_DEFAULT_HEAP_MANAGER == nullptr)
	{
		HeapManagerInit();
	}

	return HeapManagerProxy::alloc(S_DEFAULT_HEAP_MANAGER, i_size);
}

void operator delete[](void* i_ptr)
{
	if (i_ptr && S_DEFAULT_HEAP_MANAGER)
	{
		HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, i_ptr);
	}
}

void* operator new(size_t i_size, void* i_ptr)
{
	if (i_ptr)
	{
		return i_ptr;
	}
}

void* operator new(size_t i_size, NewAlignment i_alignment)
{
	if (S_DEFAULT_HEAP_MANAGER == nullptr)
	{
		HeapManagerInit();
	}

	return HeapManagerProxy::alloc(S_DEFAULT_HEAP_MANAGER, i_size, i_alignment);
}

void operator delete(void* i_ptr, NewAlignment i_alignment)
{
	if (i_ptr && S_DEFAULT_HEAP_MANAGER)
	{
		HeapManagerProxy::free(S_DEFAULT_HEAP_MANAGER, i_ptr);
	}
}