#pragma once

namespace HeapManagerController
{
	struct BlockDesc
	{
		void*			m_pBlockBase;
		size_t			m_sizeBlock;
		BlockDesc*		m_pNext;
		BlockDesc*		m_pPrev;
	};
}
