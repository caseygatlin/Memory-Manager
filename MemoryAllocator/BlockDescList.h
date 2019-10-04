#pragma once
#include "BlockDesc.h"

namespace HeapManagerController
{
	class BlockDescList
	{
	public:
		BlockDescList();
		~BlockDescList();

		BlockDesc front() const;
		BlockDesc back() const;
		BlockDesc getItem(int i_position) const;
		BlockDesc getItem(BlockDesc& i_val) const;

		void addToFront(BlockDesc i_item);
		void addToRear(BlockDesc i_item);
		void removeFront();
		void removeRear();
		void insert(int position, BlockDesc i_val);
		void deleteItem(int i_position);
		void deleteItem(BlockDesc& i_val);


	private:
		BlockDesc* m_pHead;
		BlockDesc* m_pTail;
	};
}
