#pragma once

#include <stdint.h>

struct FixedSizeAllocatorInitData
{
	FixedSizeAllocatorInitData(size_t i_sizeBlock, size_t i_numBlocks) :
		sizeBlock(i_sizeBlock),
		numBlocks(i_numBlocks)
	{}

	size_t sizeBlock;
	size_t numBlocks;
};

const static unsigned int S_NUM_FSA_INIT = 5;
static FixedSizeAllocatorInitData S_FSA_INITS[5] =
{
	FixedSizeAllocatorInitData(8, 20),
	FixedSizeAllocatorInitData(16, 20),
	FixedSizeAllocatorInitData(32, 20),
	FixedSizeAllocatorInitData(64, 20),
	FixedSizeAllocatorInitData(128, 5)
};
