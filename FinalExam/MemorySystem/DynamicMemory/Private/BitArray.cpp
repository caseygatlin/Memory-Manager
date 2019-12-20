#include "BitArray.h"
#include <stdint.h>
#include <intrin.h>


#ifdef _WIN64


#ifndef SET_BIT_VAL
#define SET_BIT_VAL UINT64_MAX
#endif // !SET_BIT_VAL

#ifndef BIT_DIVISOR
#define BIT_DIVISOR 64
#endif // !BIT_DIVISOR

#pragma intrinsic(_BitScanForward64)
typedef uint64_t UINT_TYPE;


#else


#ifndef SET_BIT_VAL
#define SET_BIT_VAL UINT32_MAX
#endif // !SET_BIT_VAL

#ifndef BIT_DIVISOR
#define BIT_DIVISOR 32
#endif // !BIT_DIVISOR

#pragma intrinsic(_BitScanForward)
typedef uint32_t UINT_TYPE;


#endif // _WIN64


BitArray* BitArray::Create(const size_t& i_numBits, const void* i_memLocation)
{

	// Convert numBits to size
	size_t arraySize = i_numBits / BIT_DIVISOR;

	if (i_numBits % BIT_DIVISOR > 0)
	{

		arraySize++;

	}


	// Defines new BitArray in memory
	BitArray* pBitArray = static_cast<BitArray*>(const_cast<void*>(i_memLocation));


	// Constructs BitArray
	uintptr_t   uip_BitArrayMem    = reinterpret_cast<uintptr_t>  (i_memLocation);
	void*       v_pBitArrayStart   = reinterpret_cast<void*>      (uip_BitArrayMem + sizeof(BitArray));
			    *pBitArray         = BitArray(arraySize, v_pBitArrayStart);


	return pBitArray;

}



// Constructor
BitArray::BitArray(const size_t& i_size, void* i_memLocation)
{

	m_size    = i_size;
	m_pBits   = static_cast<UINT_TYPE*>(i_memLocation);

	for (size_t i = 0; i < i_size; i++)
	{

		m_pBits[i] = SET_BIT_VAL;

	}

}



BitArray::~BitArray()
{

	m_pBits   = nullptr;
	m_size    = 0;
		
}



void BitArray::ClearAll()
{

	for (size_t i = 0; i < m_size; i++)
	{

		m_pBits[i] = 0x00;
			
	}
}



void BitArray::SetAll()
{

	for (size_t i = 0; i < m_size; i++)
	{
				
		m_pBits[i] = SET_BIT_VAL;
			
	}
}



bool BitArray::IsBitSet(size_t i_bitIndex) const
{

	size_t          index       = i_bitIndex / BIT_DIVISOR;
	size_t          bitIndex    = i_bitIndex % BIT_DIVISOR;
	const long long targetInt   = static_cast<const long long>(m_pBits[index]);

#ifdef _WIN64
	return (_bittest64(&targetInt, bitIndex));
#else
	return (_bittest(&targetInt, bitIndex));
#endif // _WIN64

}



bool BitArray::IsBitClear(size_t i_bitIndex) const
{
	
	size_t          index       = i_bitIndex / BIT_DIVISOR;
	size_t          bitIndex    = i_bitIndex % BIT_DIVISOR;
	const long long targetInt   = static_cast<const long long>(m_pBits[index]);

#ifdef _WIN64
	return !(_bittest64(&targetInt, bitIndex));
#else
	return !(_bittest(&targetInt, bitIndex));
#endif // _WIN64

}



void BitArray::SetBit(size_t i_bitIndex)
{
			
	size_t    index       = i_bitIndex / BIT_DIVISOR;
	size_t    bitIndex    = i_bitIndex % BIT_DIVISOR;
	long long targetInt   = static_cast<long long>(m_pBits[index]);

#ifdef _WIN64
	_bittestandset64(&targetInt, bitIndex);
#else
	_bittestandset(&targetInt, bitIndex);
#endif // _WIN64

	m_pBits[index] = targetInt;

}



void BitArray::ClearBit(size_t i_bitIndex)
{

	size_t    index       = i_bitIndex / BIT_DIVISOR;
	size_t    bitIndex    = i_bitIndex % BIT_DIVISOR;
	long long targetInt   = static_cast<long long>(m_pBits[index]);

#ifdef _WIN64
	_bittestandreset64(&targetInt, bitIndex);
#else
	_bittestandreset(&targetInt, bitIndex);
#endif // _WIN64

	m_pBits[index] = targetInt;

}



bool BitArray::FindFirstSetBit(size_t& o_bitIndex) const
{
	size_t index = 0;

	while (m_pBits[index] == 0x00 && index < m_size)
	{
		
		index++;
			
	}

	unsigned long bitIndex = static_cast<unsigned long>(o_bitIndex);

#ifdef _WIN64
	bool hasSetBit = _BitScanForward64(&bitIndex, m_pBits[index]);
#else
	bool hasSetBit = _BitScanForward(&bitIndex, m_pBits[index]);
#endif // _WIN64


	o_bitIndex = static_cast<size_t>(bitIndex);

	return hasSetBit;
		
}



bool BitArray::FindFirstClearBit(size_t& o_bitIndex) const
{
			
	size_t index = 0;

	while (m_pBits[index] == SET_BIT_VAL && index < m_size)
	{

		index++;

	}

	size_t bitIndex = 0;
	const long long targetInt = static_cast<const long long>(m_pBits[index]);

	

#ifdef _WIN64

	while (_bittest64(&targetInt, bitIndex) && bitIndex < BIT_DIVISOR)
	{
		bitIndex++;
	}

#else
	while (_bittest(&targetInt, bitIndex) && bitIndex < BIT_DIVISOR)
	{
		bitIndex++;
	}

#endif // _WIN64

	o_bitIndex = bitIndex + index * BIT_DIVISOR;

	return true;

}



bool BitArray::GetNextSetBit(size_t& o_bitIndex) const
{

	return true;

}



bool BitArray::GetNextClearBit(size_t& o_bitIndex) const
{
	return true;
}

