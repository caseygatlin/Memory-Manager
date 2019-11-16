#pragma once

class HeapManager;

static	HeapManager* S_DEFAULT_HEAP_MANAGER = nullptr;
void	HeapManagerInit();


// Non-array new & delete
void*	operator new(size_t i_size);
void	operator delete(void* i_ptr);

// Array new & delete
void*	operator new[](size_t i_size);
void	operator delete[](void* i_ptr);

// Placement new
void*	operator new(size_t i_size, void* i_ptr);


// Enum for aligned new & delete
enum NewAlignment
{
	NEW_ALIGN_DEFAULT = 4,
	NEW_ALIGN_16 = 16,
	NEW_ALIGN_32 = 32,
	NEW_ALIGN_64 = 64
};


// Aligned new & delete
void*	operator new(size_t i_size, NewAlignment i_alignment);
void	operator delete(void* i_ptr, NewAlignment i_alignment);
