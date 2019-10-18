#include "HeapManagerProxy.h"
#include "HeapManager.h"
#include "HeapManager_UnitTest.h"
#include "BlockDesc.h"

#include <Windows.h>

#include <assert.h>
#include <algorithm>
#include <vector>
#include <iostream>


//#define SUPPORTS_ALIGNMENT
#define SUPPORTS_SHOWFREEBLOCKS
#define SUPPORTS_SHOWOUTSTANDINGALLOCATIONS

#define USE_HEAP_ALLOC

bool HeapManager_UnitTest_Allocate()
{
	std::cout << "This unit test allocates memory for 5 variables of different types.\n"
		<< "Afterwards, it prints out the resulting block descriptors.\n"
		<< "NOTE: Allocations are in 4-byte alignment and have two 4-byte guard bands.\n\n\n";

	using namespace HeapManagerProxy;

	const size_t 		sizeHeap = 1024 * 1024;

	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);

	assert(pHeapMemory);

	HeapManager * pHeapManager = CreateHeapManager(pHeapMemory, sizeHeap);
	assert(pHeapManager);

	if (pHeapManager == nullptr)
		return false;

	std::cout << "Size of Block Descriptor: " << sizeof(BlockDesc) << std::endl;
	std::cout << "Size of Guard Band: " << 4 << std::endl;
	std::cout << "Size of Int: " << sizeof(int) << std::endl;
	std::cout << "Size of Bool: " << sizeof(bool) << std::endl;
	std::cout << "Size of Double: " << sizeof(double) << std::endl;
	std::cout << "Size of Char: " << sizeof(char) << std::endl;
	std::cout << "Size of Float: " << sizeof(float) << std::endl << std::endl;



	// Before allocations
	std::cout << "********** BEFORE ALLOCATIONS **********" << std::endl << std::endl;
	ShowFreeBlocks(pHeapManager);

	// Int allocation
	int* my_int = static_cast<int*>(alloc(pHeapManager, sizeof(int)));

	// After Int
	std::cout << "********** AFTER INT ALLOCATION **********" << std::endl << std::endl;
	ShowFreeBlocks(pHeapManager);
	ShowOutstandingAllocations(pHeapManager);

	// Bool allocation
	bool* my_bool = static_cast<bool*>(alloc(pHeapManager, sizeof(bool)));

	// After Bool
	std::cout << "********** AFTER BOOL ALLOCATION **********" << std::endl << std::endl;
	ShowFreeBlocks(pHeapManager);
	ShowOutstandingAllocations(pHeapManager);

	// Double allocation
	double* my_double = static_cast<double*>(alloc(pHeapManager, sizeof(double)));

	// After Double
	std::cout << "********** AFTER DOUBLE ALLOCATION **********" << std::endl << std::endl;
	ShowFreeBlocks(pHeapManager);
	ShowOutstandingAllocations(pHeapManager);

	// Char allocation
	char* my_char = static_cast<char*>(alloc(pHeapManager, sizeof(char)));

	// After char
	std::cout << "********** AFTER CHAR ALLOCATION **********" << std::endl << std::endl;
	ShowFreeBlocks(pHeapManager);
	ShowOutstandingAllocations(pHeapManager);

	// Float allocation
	float* my_float = static_cast<float*>(alloc(pHeapManager, sizeof(float)));

	// After Float
	std::cout << "********** AFTER FLOAT ALLOCATION **********" << std::endl << std::endl;
	ShowFreeBlocks(pHeapManager);
	ShowOutstandingAllocations(pHeapManager);

	return true;
}


bool HeapManager_UnitTest()
{
	using namespace HeapManagerProxy;

	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048; //TODO: Remove numDescriptors

#ifdef USE_HEAP_ALLOC
	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
#else
	// Get SYSTEM_INFO, which includes the memory page size
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	// round our size to a multiple of memory page size
	assert(SysInfo.dwPageSize > 0);
	size_t sizeHeapInPageMultiples = SysInfo.dwPageSize * ((sizeHeap + SysInfo.dwPageSize) / SysInfo.dwPageSize);
	void* pHeapMemory = VirtualAlloc(NULL, sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#endif
	assert(pHeapMemory);

	// Create a heap manager for my test heap.
	HeapManager * pHeapManager = CreateHeapManager(pHeapMemory, sizeHeap);
	assert(pHeapManager);

	if( pHeapManager == nullptr )
		return false;

#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
#ifdef SUPPORTS_SHOWFREEBLOCKS
		ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS

		size_t largestBeforeAlloc = GetLargestFreeBlock(pHeapManager);
		void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);

		if( pPtr )
		{
#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
			printf("After large allocation:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
			ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS
#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			printf("\n");
#endif

			size_t largestAfterAlloc = GetLargestFreeBlock(pHeapManager);
			bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
			assert(success);

			success = free(pHeapManager, pPtr);
			assert(success);

			Collect(pHeapManager);

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
			printf("After freeing allocation and garbage collection:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
			ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS
#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			printf("\n");
#endif

			size_t largestAfterCollect = GetLargestFreeBlock(pHeapManager);
		}
	}
#endif

	std::vector<void *> AllocatedAddresses;

	long	numAllocs = 0;
	long	numFrees = 0;
	long	numCollects = 0;

	// allocate memory of random sizes up to 1024 bytes from the heap manager
	// until it runs out of memory
	do
	{
		const size_t		maxTestAllocationSize = 1024;

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

#ifdef SUPPORTS_ALIGNMENT
		// pick an alignment
		const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

		const unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

		const unsigned int	alignment = alignments[index];

		void * pPtr = alloc(pHeapManager, sizeAlloc, alignment);

		// check that the returned address has the requested alignment
		assert((reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0);
#else
		void * pPtr = alloc(pHeapManager, sizeAlloc);
#endif // SUPPORT_ALIGNMENT

		// if allocation failed see if garbage collecting will create a large enough block
		if( pPtr == nullptr )
		{
			Collect(pHeapManager);

#ifdef SUPPORTS_ALIGNMENT
			pPtr = alloc(pHeapManager, sizeAlloc, alignment);
#else
			pPtr = alloc(pHeapManager, sizeAlloc);
#endif // SUPPORT_ALIGNMENT

			// if not we're done. go on to cleanup phase of test
			if( pPtr == nullptr )
				break;
		}

		AllocatedAddresses.push_back(pPtr);
		numAllocs++;

		// randomly free and/or garbage collect during allocation phase
		const unsigned int freeAboutEvery = 10;
		const unsigned int garbageCollectAboutEvery = 40;

		if( !AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0) )
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
			assert(success);

			success = free(pHeapManager, pPtr);
			assert(success);

			numFrees++;
		}

		if( (rand() % garbageCollectAboutEvery) == 0 )
		{
			Collect(pHeapManager);

			numCollects++;
		}

	} while( 1 );

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
	printf("After exhausting allocations:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
	ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS
#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
	ShowOutstandingAllocations(pHeapManager);
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
	printf("\n");
#endif

	// now free those blocks in a random order
	if( !AllocatedAddresses.empty() )
	{
		// randomize the addresses
		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

		// return them back to the heap manager
		while( !AllocatedAddresses.empty() )
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
			assert(success);

			success = free(pHeapManager, pPtr);
			assert(success);
		}

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
		printf("After freeing allocations:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
		ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS

#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
		ShowOutstandingAllocations(pHeapManager);
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
		printf("\n");
#endif

		// do garbage collection
		Collect(pHeapManager);
		// our heap should be one single block, all the memory it started with

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
		printf("After garbage collection:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
		ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS

#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
		ShowOutstandingAllocations(pHeapManager);
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
		printf("\n");
#endif

		// do a large test allocation to see if garbage collection worked
		void * pPtr = alloc(pHeapManager, sizeHeap / 2);
		assert(pPtr);

		if( pPtr )
		{
			bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
			assert(success);

			success = free(pHeapManager, pPtr);
			assert(success);

		}
	}

	Destroy(pHeapManager);
	pHeapManager = nullptr;

	if (pHeapMemory)
	{
#ifdef USE_HEAP_ALLOC
		HeapFree(GetProcessHeap(), 0, pHeapMemory);
#else
		VirtualFree(pHeapMemory, 0, MEM_RELEASE);
#endif
	}

	// we succeeded
	return true;
}
