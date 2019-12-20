#include "BitArray.h"
#include <intrin.h>
#include <stdint.h>


// Preprocessor definitions, types, and intrinsics for WIN64
#ifdef _WIN64

#ifndef SET_BIT_VAL
#define SET_BIT_VAL UINT64_MAX
#endif // !SET_BIT_VAL

#ifndef BIT_DIVISOR
#define BIT_DIVISOR 64
#endif // !BIT_DIVISOR

#pragma intrinsic(_BitScanForward64)
typedef uint64_t UINT_TYPE;


// Preprocessor definitions, types, and intrinsics for WIN32
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



// Constructs and returns a BitArray*
BitArray* BitArray::Create(const size_t& i_numBits, const void* i_memLocation)
{

	// Convert numBits to size
	size_t arraySize = i_numBits / BIT_DIVISOR;


    // Adds 1 to size of array if there's a remainder
	if (i_numBits % BIT_DIVISOR > 0)
	{

		arraySize++;

	}


	// Defines new BitArray in memory
	BitArray*   pBitArray          = static_cast<BitArray*>(const_cast<void*>(i_memLocation));


	// Constructs BitArray
	uintptr_t   uip_BitArrayMem    = reinterpret_cast<uintptr_t>  (i_memLocation);
	void*       v_pBitArrayStart   = reinterpret_cast<void*>      (uip_BitArrayMem + sizeof(BitArray));
			    *pBitArray         = BitArray(arraySize, v_pBitArrayStart);


	return pBitArray;

}



// Constructor
BitArray::BitArray(const size_t& i_size, void* i_memLocation)
{

    // Set size and array ptr
	m_size    = i_size;
	m_pBits   = static_cast<UINT_TYPE*>(i_memLocation);


    // Initialize array with set bits
	for (size_t i = 0; i < i_size; i++)
	{

		m_pBits[i] = SET_BIT_VAL;

	}

}



// Sets all bits to 0
void BitArray::ClearAll()
{

	for (size_t i = 0; i < m_size; i++)
	{

		m_pBits[i] = 0x00;
			
	}

}



// Sets all bits to 1
void BitArray::SetAll()
{

	for (size_t i = 0; i < m_size; i++)
	{
				
		m_pBits[i] = SET_BIT_VAL;
			
	}

}



// Returns true if a given bit is 1, false otherwise
bool BitArray::IsBitSet(size_t i_bitIndex) const
{

    // Convert a bit index into a uint + bit index
	size_t          index       = i_bitIndex / BIT_DIVISOR;
	size_t          bitIndex    = i_bitIndex % BIT_DIVISOR;

    
    // Gets the bit and returns it
#ifdef _WIN64

    const long long targetInt   = static_cast<const long long>  (m_pBits[index]);
    long long ll_bitIndex       = static_cast<long long>        (bitIndex);

	return static_cast<bool>((_bittest64(&targetInt, ll_bitIndex)));

#else

    const long targetInt    = static_cast<const long>   (m_pBits[index]);
    long l_bitIndex         = static_cast<long>         (bitIndex);

	return static_cast<bool>((_bittest(&targetInt, l_bitIndex)));

#endif // _WIN64

}



// Returns true if a given bit is 0, false otherwise
bool BitArray::IsBitClear(size_t i_bitIndex) const
{
	
    // Convert a bit index into a uint + bit index
	size_t          index       = i_bitIndex / BIT_DIVISOR;
	size_t          bitIndex    = i_bitIndex % BIT_DIVISOR;


    // Get the bit and return its complement
#ifdef _WIN64

    const long long targetInt   = static_cast<const long long>  (m_pBits[index]);
    long long ll_bitIndex       = static_cast<long long>        (bitIndex);

	return !(_bittest64(&targetInt, ll_bitIndex));

#else

    const long targetInt    = static_cast<const long>   (m_pBits[index]);
    long l_bitIndex         = static_cast<long>         (bitIndex);

	return !(_bittest(&targetInt, l_bitIndex));

#endif // _WIN64

}



// Sets a given bit to 1
void BitArray::SetBit(size_t i_bitIndex)
{
	
    // Convert a bit index into a uint + bit index
	size_t    index       = i_bitIndex / BIT_DIVISOR;
	size_t    bitIndex    = i_bitIndex % BIT_DIVISOR;


    // Set the given bit to 1
#ifdef _WIN64

    long long targetInt     = static_cast<long long>(m_pBits[index]);
    long long ll_bitIndex   = static_cast<long long>(bitIndex);

	_bittestandset64(&targetInt, ll_bitIndex);

#else

    long targetInt  = static_cast<long>(m_pBits[index]);
    long l_bitIndex = static_cast<long>(bitIndex);


	_bittestandset(&targetInt, l_bitIndex);

#endif // _WIN64


    // Replace the value in the array
	m_pBits[index] = targetInt;

}



// Sets a given bit to 0
void BitArray::ClearBit(size_t i_bitIndex)
{

    // Convert a bit index into a uint + bit index
	size_t    index       = i_bitIndex / BIT_DIVISOR;
	size_t    bitIndex    = i_bitIndex % BIT_DIVISOR;


    // Set the given bit to 0
#ifdef _WIN64

    long long targetInt     = static_cast<long long>(m_pBits[index]);
    long long ll_bitIndex   = static_cast<long long>(bitIndex);

	_bittestandreset64(&targetInt, ll_bitIndex);

#else

    long targetInt  = static_cast<long>(m_pBits[index]);
    long l_bitIndex = static_cast<long>(bitIndex);

	_bittestandreset(&targetInt, l_bitIndex);

#endif // _WIN64


    // Replace the value in the array
	m_pBits[index] = targetInt;

}



// Find the first 1 bit in the array
bool BitArray::FindFirstSetBit(size_t& o_bitIndex) const
{

    // Cycle through clear bits
	size_t index = 0;

	while (m_pBits[index] == 0x00 && index < m_size)
	{
		
		index++;
			
	}

	unsigned long bitIndex = static_cast<unsigned long>(o_bitIndex);


    // Return whether there's a 1 and modify given reference
#ifdef _WIN64

	bool hasSetBit = static_cast<bool>(_BitScanForward64(&bitIndex, m_pBits[index]));

#else

	bool hasSetBit = static_cast<bool>(_BitScanForward(&bitIndex, m_pBits[index]));

#endif // _WIN64


	o_bitIndex = static_cast<size_t>(bitIndex);

	return hasSetBit;
		
}



// Find the first 0 bit in the array
bool BitArray::FindFirstClearBit(size_t& o_bitIndex) const
{
	
    // Cycle through the set bits
	size_t index = 0;
    bool hasClearBit = false;
	
    for (size_t i = 0; i < m_size; i++)
    {
        if (m_pBits[i] == 0x00)
        {
            hasClearBit = true;
            index = i;
            break;
        }
    }

    
    // Check whether the last uint is set or not
	if (!hasClearBit)
	{
		return false;
	}


	size_t bitIndex = 0;

	
    // Cycle through each bit in the uint to find the clear bit
#ifdef _WIN64

    const long long targetInt = static_cast<const long long>(m_pBits[index]);

    for (size_t i = 0; i < BIT_DIVISOR; i++)
    {
        if (!(_bittest64(&targetInt, i)))
        {
            bitIndex = i;
            break;
        }
    }


#else

    const long targetInt = static_cast<const long>(m_pBits[index]);

    for (size_t i = 0; i < BIT_DIVISOR; i++)
    {
        if (!(_bittest(&targetInt, i)))
        {
            bitIndex = i;
            break;
        }
    }

#endif // _WIN64


    // Modify the given reference
	o_bitIndex = bitIndex + index * BIT_DIVISOR;

	return true;

}
