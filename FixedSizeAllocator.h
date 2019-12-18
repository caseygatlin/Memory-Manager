#pragma once

class BitArray;

class FixedSizeAllocator
{
public:
	FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize);

	static FixedSizeAllocator* Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize);
	~FixedSizeAllocator();

	void* _alloc();
	bool _free(void* i_ptr);
	

private:
	size_t m_BlockSize;
	void* m_pFreeMem;
	BitArray* m_pFreeBits;
};