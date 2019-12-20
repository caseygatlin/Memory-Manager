#pragma once




class BitArray;

class FixedSizeAllocator
{
public:
	static FixedSizeAllocator* Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize);

	FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize);
	~FixedSizeAllocator();

	void*	_alloc();
	bool	_free(void* i_ptr);

	bool Contains(void* i_ptr) const;

	size_t	GetBlockSize() const;


private:
	size_t		m_memorySize;
	size_t		m_BlockSize;
	void*		m_pFreeMem;
	BitArray*	m_pFreeBits;

};
