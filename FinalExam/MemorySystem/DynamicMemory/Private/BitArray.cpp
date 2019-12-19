#include "BitArray.h"
#include <stdint.h>

#ifdef _WIN64
#define SET_BIT_VAL UINT64_MAX
#define BIT_DIVISOR 64
typedef uint64_t UINT_TYPE;
#else
#define SET_BIT_VAL UINT32_MAX
#define BIT_DIVISOR 32
typedef uint32_t UINT_TYPE;
#endif // _WIN64

namespace memory_system
{
	namespace dynamic_memory
	{

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
			uintptr_t		uip_BitArrayMem = reinterpret_cast<uintptr_t>(i_memLocation);
			void* v_pBitArrayStart = reinterpret_cast<void*>(uip_BitArrayMem + sizeof(BitArray));
			*pBitArray = BitArray(arraySize, v_pBitArrayStart);

			return pBitArray;
		}

		// Constructor
		BitArray::BitArray(const size_t& i_size, void* i_memLocation)
		{
			m_size = i_size;

			m_pBits = static_cast<UINT_TYPE*>(i_memLocation);

			for (size_t i = 0; i < i_size; i++)
			{
				m_pBits[i] = SET_BIT_VAL;
			}

		}

		BitArray::~BitArray()
		{
			m_pBits = nullptr;
			m_size = 0;
		}

		void BitArray::ClearAll()
		{
			for (size_t i = 0; i < m_size; i++)
			{
				m_pBits[i] = 0;
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
			size_t index = i_bitIndex / BIT_DIVISOR;
			size_t bitIndex = i_bitIndex % BIT_DIVISOR;

#ifdef _WIN64
			uint64_t targetInt = m_pBits[index];
			// Find the bit and whether it's set
#else
			uint32_t targetInt = m_pBits[index];
			// Find the bit and whether it's set

#endif // _WIN64

			return true;

		}

		bool BitArray::IsBitClear(size_t i_bitIndex) const
		{
			size_t index = i_bitIndex / BIT_DIVISOR;
			size_t bitIndex = i_bitIndex % BIT_DIVISOR;

#ifdef _WIN64
			uint64_t targetInt = m_pBits[index];
			// Find the bit and whether it's clear
#else
			uint32_t targetInt = m_pBits[index];
			// Find the bit and whether it's clear

#endif // _WIN64

			return true;
		}

		void BitArray::SetBit(size_t i_bitIndex)
		{
			size_t index = i_bitIndex / BIT_DIVISOR;
			size_t bitIndex = i_bitIndex % BIT_DIVISOR;

#ifdef _WIN64
			uint64_t targetInt = m_pBits[index];
			// Set the bit

#else
			uint32_t targetInt = m_pBits[index];
			// Set the bit

#endif // _WIN64

		}

		void BitArray::ClearBit(size_t i_bitIndex)
		{
			size_t index = i_bitIndex / BIT_DIVISOR;
			size_t bitIndex = i_bitIndex % BIT_DIVISOR;

#ifdef _WIN64
			uint64_t targetInt = m_pBits[index];
			// Clear the bit

#else
			uint32_t targetInt = m_pBits[index];
			// Clear the bit

#endif // _WIN64
		}


		bool BitArray::FindFirstSetBit(size_t& o_bitIndex) const
		{
			size_t index = 0;

			while (m_pBits[index] == 0 && index < m_size)
			{
				index++;
			}

			//Findsetbit and return

			return true;
		}

		bool BitArray::FindFirstClearBit(size_t& o_bitIndex) const
		{
			size_t index = 0;

			while (m_pBits[index] == SET_BIT_VAL && index < m_size)
			{
				index++;
			}

			//findclearbit and return
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
	}
}
