#include "pch.h"
#include "MemoryPool.h"

/*--------------
	MemoryPool
---------------*/


// MemoryPool ��ü�� �ʱ�ȭ
// allocSize�� �޸� Ǯ���� �Ҵ��� �޸� ����� ũ��
MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

// �޸� Ǯ ��ü�� �Ҹ�� �� ȣ��
// ť�� �����ִ� ��� �޸� ����� ����
MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

// �޸� ����� �޸� Ǯ�� ��ȯ
// WRITE_LOCK�� ����Ͽ� ������ �������� ����
// ��ȯ�� �޸� ����� allocSize�� 0���� �����ϰ�, ť�� �߰�
// _allocCount�� ����
void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	ptr->allocSize = 0;

	
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_allocCount.fetch_sub(1);
}


// �޸� Ǯ���� �޸� ����� �����´�.
// ť�� ������ �޸� ����� ������ ��������, ������ ���� �Ҵ�
// _allocCount�� ����
MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// ������ ���� �����
	if (memory == nullptr)
	{
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
	}

	_allocCount.fetch_add(1);

	return memory;

}
