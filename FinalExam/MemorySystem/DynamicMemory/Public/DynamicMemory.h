#pragma once


class FixedSizeAllocator;
class HeapManager;

HeapManager*	GetDefaultHeapManager();



void					CreateFixedSizeAllocator(const size_t& i_sizeBlock, const size_t& i_numBlocks, HeapManager* i_pHeapManager);
HeapManager*			CreateDefaultHeap(void* i_pHeapMemory, size_t i_sizeHeapMemory);
FixedSizeAllocator*		FindFixedSizeAllocator(const size_t& i_sizeBlock);

