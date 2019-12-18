#pragma once

namespace MemorySystemProxy
{
	struct FSAInitData
	{
		FSAInitData(size_t i_sizeBlock, size_t i_numBlocks) :
			sizeBlock(i_sizeBlock),
			numBlocks(i_numBlocks)
		{}

		size_t sizeBlock;
		size_t numBlocks;
	};

	const static unsigned int S_NUM_FSA_INIT = 5;
	static FSAInitData S_FSA_INITS[5] =
	{
		FSAInitData(8, 100),
		FSAInitData(16, 100),
		FSAInitData(32, 200),
		FSAInitData(64, 100),
		FSAInitData(128, 50)
	};
}