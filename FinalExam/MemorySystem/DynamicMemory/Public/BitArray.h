#pragma once

#include <stdint.h>


		class FixedSizeAllocator;

		class BitArray
		{
		public:

			static BitArray* Create(const size_t& i_numBits, const void* i_memLocation);

			// Constructor
			BitArray(const size_t& i_size, void* i_memLocation);

			// Destructor
			~BitArray();

			void ClearAll();
			void SetAll();

			bool IsBitSet(size_t i_bitIndex) const;
			bool IsBitClear(size_t i_bitIndex) const;

			void SetBit(size_t i_bitIndex);
			void ClearBit(size_t i_bitIndex);


			// Finds and outputs index of first set bit
			bool FindFirstSetBit(size_t& o_bitIndex) const;
			bool FindFirstClearBit(size_t& o_bitIndex) const;

			bool GetNextSetBit(size_t& o_bitIndex) const;
			bool GetNextClearBit(size_t& o_bitIndex) const;

			// Operator[]

		private:

			// Array of bits depending on architecture
#ifdef _WIN64
			uint64_t* m_pBits;
#else
			uint32_t* m_pBits;
#endif

			// Size of the array
			size_t m_size;

		};
