#include "pch.h"
#include "MemoryPool.h"

/*--------------
	MemoryPool
---------------*/


// MemoryPool 객체를 초기화
// allocSize는 메모리 풀에서 할당할 메모리 블록의 크기
MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

// 메모리 풀 객체가 소멸될 때 호출
// 큐에 남아있는 모든 메모리 블록을 해제
MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

// 메모리 블록을 메모리 풀을 반환
// WRITE_LOCK을 사용하여 스레드 안정성을 보장
// 반환된 메모리 블록의 allocSize을 0으로 설정하고, 큐에 추가
// _allocCount를 감소
void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	ptr->allocSize = 0;

	
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_allocCount.fetch_sub(1);
}


// 메모리 풀에서 메모리 블록을 가져온다.
// 큐에 여분의 메모리 블록이 있으면 꺼내오고, 없으면 새로 할당
// _allocCount를 증가
MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// 없으면 새로 만든다
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
