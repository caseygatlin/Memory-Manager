#include "BitArray.h"
#include <stdint.h>

// Constructor
BitArray::BitArray(const size_t& i_size, void* i_memLocation)
{
	m_size = i_size;


#ifdef _WIN64
	m_pBits = static_cast<uint64_t*>(i_memLocation);
	for (size_t i = 0; i < i_size; i++)
	{
		m_pBits[i] = UINT64_MAX;
	}

#else
	m_pBits = static_cast<uint32_t*>(i_memLocation);
	for (size_t i = 0; i < i_size; i++)
	{
		m_pBits[i] = UINT32_MAX;
	}

#endif

}

BitArray::BitArray(const BitArray& i_src)
{

}

BitArray& BitArray::operator=(const BitArray& i_src)
{

}

BitArray::~BitArray()
{
	delete[] m_pBits;
}

bool BitArray::FindFirstSetBit(size_t& o_bitIndex)
{

}