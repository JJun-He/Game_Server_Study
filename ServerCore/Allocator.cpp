#include "pch.h"
#include "Allocator.h"
#include "Memory.h"

/*--------------------
:	 BaseAllocator
----------------------*/

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}

/*-------------------
:   StompAllocator
---------------------*/

// ��ȯ�� : �Ҵ�� �޸��� ���� �ּҿ� �������� ���� ���� ��ȯ
void* StompAllocator::Alloc(int32 size)
{
	// �� ������ ����� ����� �Ǿ� ����
	// ������ �޸� ������ ������ �� �ִ� ��ġ
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	// ��ǥ�� �����(�Ҵ�� �������� ������ ����� ������ ����) => ���� ��ħ
	const int64 dataOffset = pageCount * PAGE_SIZE - size;

	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	// �ٸ� Ÿ���̴� ���� ĳ������ �ؼ� ������ ���
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

// ��� : �־��� �����Ͱ� ����Ű�� �޸𸮸� ������ ������ ����
// ���� : VirtualFree �Լ��� ���
void StompAllocator::Release(void* ptr)
{
	// ������ �ּҸ� ������ �ٷ�� ����
	const int64 address = reinterpret_cast<int64>(ptr);
	// ptr�� ���� �������� ���� �ּҸ� ���� => �̴� ������ ��迡 ���� �޸𸮸� �����ϱ� ����
	const int64 baseAddress = address - (address % PAGE_SIZE);
	// baseAddress�� ����Ű�� �������� ���� => MEM_RELEASE �÷��״� ��ü �޸� ������ ����
	// ���ǻ���: VirtualFree �Լ��� VirtualAlloc���� �Ҵ�� �޸𸮸� �����ϴµ� ���
	// ptr�� VirtualAlloc���� �Ҵ�� �޸𸮰� �ƴ� ���, ������ ���ǵ��� ���� �� ����
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}

/*-------------------
:   PoolAllocator
---------------------*/

void* PoolAllocator::Alloc(int32 size)
{
	return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	GMemory->Release(ptr);
}
