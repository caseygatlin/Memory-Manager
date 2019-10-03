#pragma once

struct BlockDescriptor
{
	void *				m_pBlockBase;
	size_t				m_sizeBlock;
	BlockDescriptor *	m_pNext;
	BlockDescriptor *	m_pPrev;
};