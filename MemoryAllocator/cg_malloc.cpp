
#include "BlockDescriptor.h"
#include <heapapi.h>
#include "cg_malloc.h"
#include "DoublyLinkedList.h"

void* cg_malloc(size_t size)
{
	// Return nullptr if size <= 0
	if (size <= 0)
		return nullptr;

}