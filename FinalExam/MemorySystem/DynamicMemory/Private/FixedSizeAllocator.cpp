#include "FixedSizeAllocator.h"
#include "BitArray.h"
#include <stdint.h>



		FixedSizeAllocator::FixedSizeAllocator(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize)
		{

			m_BlockSize   = i_blockSize;
			m_pFreeMem    = const_cast<void*>(i_pHeapMemory);
			

			uintptr_t   uip_BitArrayStart   = reinterpret_cast<uintptr_t>(m_pFreeMem);
			            uip_BitArrayStart   += i_numBlocks * i_blockSize;


			m_pFreeBits = BitArray::Create(i_numBlocks, reinterpret_cast<void*>(uip_BitArrayStart));

		}



		FixedSizeAllocator* FixedSizeAllocator::Create(const void* i_pHeapMemory, const size_t& i_heapMemorySize, const size_t& i_numBlocks, const size_t& i_blockSize)
		{

			// Defines new FSA in memory
			FixedSizeAllocator* pFSA = static_cast<FixedSizeAllocator*>(const_cast<void*>(i_pHeapMemory));


			// Constructs FSA
			uintptr_t   uip_FSAMemory   = reinterpret_cast<uintptr_t>  (i_pHeapMemory);
			void*       v_pFSAStart     = reinterpret_cast<void*>      (uip_FSAMemory + sizeof(FixedSizeAllocator));
			            *pFSA           = FixedSizeAllocator(v_pFSAStart, i_heapMemorySize, i_numBlocks, i_blockSize);


			return pFSA;

		}



		FixedSizeAllocator::~FixedSizeAllocator()
		{

			size_t firstClear;

			if (m_pFreeBits->FindFirstClearBit(firstClear))
			{

				// Outstanding allocation

			}

		}



		void* FixedSizeAllocator::_alloc()
		{

			size_t firstAvailable;
			if (m_pFreeBits->FindFirstSetBit(firstAvailable))
			{

				m_pFreeBits->ClearBit(firstAvailable);

				uintptr_t   uipFreeMem   = reinterpret_cast<uintptr_t>  (m_pFreeMem);
				uintptr_t   uipUserPtr   = uipFreeMem + (firstAvailable * m_BlockSize);
				void*       pUserPtr     = reinterpret_cast<void*>      (uipUserPtr);

				return pUserPtr;

			}

			else
			{

				return nullptr;

			}

		}



		bool FixedSizeAllocator::_free(void* i_ptr)
		{

			// Check that pointer is a valid pointer within this allocator
			// And that it's an outstanding allocation
			if (i_ptr)
			{

			}

			// Set bit for that block

bool FixedSizeAllocator::Contains(void* i_ptr) const
{
	uintptr_t uip_Ptr = reinterpret_cast<uintptr_t>(i_ptr);
	uintptr_t uip_FreeMem = reinterpret_cast<uintptr_t>(m_pFreeMem);
	if (uip_Ptr >= uip_FreeMem && uip_Ptr < uip_FreeMem + m_memorySize)
	{
		return true;
	}

	return false;
}



		size_t FixedSizeAllocator::GetBlockSize() const
		{

			return m_BlockSize;

		}
