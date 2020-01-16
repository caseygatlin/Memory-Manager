#pragma once

#include <stdint.h>


// Stores data for initializing FSA's
struct FixedSizeAllocatorInitData
{

    // Constructor
	FixedSizeAllocatorInitData(size_t i_sizeBlock, size_t i_numBlocks) :
		sizeBlock(i_sizeBlock),
		numBlocks(i_numBlocks)
	{}


    // Size of block and how many
	size_t sizeBlock;
	size_t numBlocks;
};



// Total number of FSA's to create
const static unsigned int S_NUM_FSA_INIT = 3;


// Data to initialize FSA's
static FixedSizeAllocatorInitData S_FSA_INITS[3] =
{

	FixedSizeAllocatorInitData(16, 100),
	FixedSizeAllocatorInitData(32, 200),
	FixedSizeAllocatorInitData(96, 400)

};
