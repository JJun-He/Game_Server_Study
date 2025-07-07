#include "pch.h"
#include "LockFreeStack.h"

// [data][ ][ ]
// Header[ next ]

// 리스트 헤더를 초기화
//void InitializeHead(SListHeader* header)
//{
//	header->next = nullptr;
//}
//
//// 리스트의 맨 앞에 새로운 노드를 삽입
//// entry->next를 현재 헤더의 첫 번째 노드로 설정
//// 헤더의 첫 번째 노드를 entry로 업데이트
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	entry->next = header->next;
//	header->next = entry;
//}
//
//// 리스트의 맨 앞에서 노드를 제거하고 반환
//// 헤더의 첫 번째 노드를 first로 설정
//// 헤더의 첫 번째 노드를 first->next로 업데이트
//SListEntry* PopEntrySList(SListHeader* header)
//{
//	SListEntry* first = header->next;
//
//	if (first != nullptr)
//		header->next = first->next;
//
//	return first;
//}

//// 리스트 헤더를 초기화
//void InitializeHead(SListHeader* header)
//{
//	// 헤더를 빈 리스트로 만듦.
//	header->next = nullptr;
//}
//
//// 리스트의 맨 앞에 새로운 노드를 삽입
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	// 현재 헤더의 첫 번째 노드로 설정
//	entry->next = header->next;
//	// 원자적으로 헤더의 첫 번째 노드를 entry로 업데이트
//	while (::InterlockedCompareExchange64((int64*)&header->next, (int64)entry, (int64)entry->next) == 0)
//	{
//		// 반복하여 원자적으로 업데이트
//	}
//
//}
//
//// [][]
//// Header [next]
//// 리스트의 맨 앞에서 노드를 제거하고 반환 
//SListEntry* PopEntrySList(SListHeader* header)
//{
//	SListEntry* expected = header->next;
//
//	// ABA Problem
//	
//	// [5000]
//	// -> [7000]
//	// [Header(6000,1)]
//
//	// 만약에 Header가 5000이라면, Header에다 6000을 넣어줘!
//	// [5000] -> [6000] -> [7000]
//	// [Header]
//	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected) == 0)
//	{
//
//	}
//
//	return expected;
//
//}


// 리스트 헤더를 초기화
void InitializeHead(SListHeader* header)
{
	// 헤더를 빈 리스트로 만듦.
	header->alignment = 0;
	header->region = 0;
}

// 리스트의 맨 앞에 새로운 노드를 삽입
void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	SListHeader expected = {};
	SListHeader desired = {};

	// 16 바이트 정렬
	desired.HeaderX64.next = (((uint64)entry) >> 4);

	while (true)
	{
		expected = *header;

		// 이 사이에 변경될 수 있다

		entry->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}
}

// [][]
// Header [next]
// 리스트의 맨 앞에서 노드를 제거하고 반환 
SListEntry* PopEntrySList(SListHeader* header)
{
	SListHeader expected = {};
	SListHeader desired = {};
	SListEntry* entry = nullptr;

	while (true)
	{
		expected = *header;

		entry = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
		if (entry == nullptr)
			break;

		desired.HeaderX64.next = ((uint64)entry->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment ,(int64*)&expected) == 1)
			break;

	}

	return entry;
}
