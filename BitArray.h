#pragma once

class uint32_t;
class uint64_t;

class BitArray
{

public:

	// Constructor
	BitArray(const size_t& i_size, void* i_memLocation);

	// Copy constructor
	BitArray(const BitArray& i_src);

	// Assignment operator
	BitArray& operator=(const BitArray& i_src);

	// Destructor
	~BitArray();


	// Finds and outputs index of first set bit
	bool FindFirstSetBit(size_t& o_bitIndex);



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