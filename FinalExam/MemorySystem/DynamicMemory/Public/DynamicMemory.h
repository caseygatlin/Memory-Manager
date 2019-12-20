#pragma once


class FixedSizeAllocator;
class HeapManager;


// --- Methods for accessing / creating memory managers --- //


// Create an FSA
void					CreateFixedSizeAllocator(const size_t& i_sizeBlock, const size_t& i_numBlocks, HeapManager* i_pHeapManager);


// Create Default Heap Manager
HeapManager*			CreateDefaultHeap(void* i_pHeapMemory, size_t i_sizeHeapMemory);


// Get Default Heap Manager
HeapManager*            GetDefaultHeapManager();


// Get an FSA that can allocate blocks of a given size
FixedSizeAllocator*		FindFixedSizeAllocator(const size_t& i_sizeBlock);

