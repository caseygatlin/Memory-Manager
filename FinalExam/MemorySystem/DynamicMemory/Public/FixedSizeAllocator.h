#pragma once


class BitArray;


// Only allocates blocks of a given size / range
class FixedSizeAllocator
{
public:

    // Creator
	static FixedSizeAllocator* Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize);

    //Constructor
	FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize);

    // Destructor
	~FixedSizeAllocator();


    // Memory management
	void*	_alloc();
	bool	_free(void* i_ptr);

    
    // Whether an address is in this manager or not
	bool    Contains(void* i_ptr) const;


    // Get the size blocks this allocates
	size_t	GetBlockSize() const;


private:

	size_t		m_memorySize;
	size_t		m_BlockSize;
	void*		m_pFreeMem;
	BitArray*	m_pFreeBits;

};
