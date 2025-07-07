#include "pch.h"
#include "LockFreeStack.h"

// [data][ ][ ]
// Header[ next ]

// ����Ʈ ����� �ʱ�ȭ
//void InitializeHead(SListHeader* header)
//{
//	header->next = nullptr;
//}
//
//// ����Ʈ�� �� �տ� ���ο� ��带 ����
//// entry->next�� ���� ����� ù ��° ���� ����
//// ����� ù ��° ��带 entry�� ������Ʈ
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	entry->next = header->next;
//	header->next = entry;
//}
//
//// ����Ʈ�� �� �տ��� ��带 �����ϰ� ��ȯ
//// ����� ù ��° ��带 first�� ����
//// ����� ù ��° ��带 first->next�� ������Ʈ
//SListEntry* PopEntrySList(SListHeader* header)
//{
//	SListEntry* first = header->next;
//
//	if (first != nullptr)
//		header->next = first->next;
//
//	return first;
//}

//// ����Ʈ ����� �ʱ�ȭ
//void InitializeHead(SListHeader* header)
//{
//	// ����� �� ����Ʈ�� ����.
//	header->next = nullptr;
//}
//
//// ����Ʈ�� �� �տ� ���ο� ��带 ����
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	// ���� ����� ù ��° ���� ����
//	entry->next = header->next;
//	// ���������� ����� ù ��° ��带 entry�� ������Ʈ
//	while (::InterlockedCompareExchange64((int64*)&header->next, (int64)entry, (int64)entry->next) == 0)
//	{
//		// �ݺ��Ͽ� ���������� ������Ʈ
//	}
//
//}
//
//// [][]
//// Header [next]
//// ����Ʈ�� �� �տ��� ��带 �����ϰ� ��ȯ 
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
//	// ���࿡ Header�� 5000�̶��, Header���� 6000�� �־���!
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


// ����Ʈ ����� �ʱ�ȭ
void InitializeHead(SListHeader* header)
{
	// ����� �� ����Ʈ�� ����.
	header->alignment = 0;
	header->region = 0;
}

// ����Ʈ�� �� �տ� ���ο� ��带 ����
void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	SListHeader expected = {};
	SListHeader desired = {};

	// 16 ����Ʈ ����
	desired.HeaderX64.next = (((uint64)entry) >> 4);

	while (true)
	{
		expected = *header;

		// �� ���̿� ����� �� �ִ�

		entry->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}
}

// [][]
// Header [next]
// ����Ʈ�� �� �տ��� ��带 �����ϰ� ��ȯ 
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
