#pragma once

class uint32_t;
class uint64_t;
class FixedSizeAllocator;

class BitArray
{

public:
	static BitArray * Create(const size_t& i_size, FixedSizeAllocator* i_pAllocator);

	// Constructor
	BitArray(const size_t& i_size, void* i_memLocation);

	// Copy constructor
	BitArray(const BitArray& i_src);

	// Assignment operator
	BitArray& operator=(const BitArray& i_src);

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