#pragma once
#include "Allocator.h"
#include "MemoryPool.h"

class MememoryPool;

/*-----------------
	Memory
------------------*/

// Memory 클래스는 메모리 풀을 관리하고, 메모리 할당과 해제를 처리하는 역할
class Memory
{
	enum
	{
		// ~1024까지 32단위, ~2048까지 128단위, ~4096까지 256단위
		POOL_COUNT = (1024 / 32) * (1024 / 128) * (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	// 주어진 크기의 메모리를 할당
	// 메모리 풀에서 메모리를 가져오거나 최대 크기를 초고하는 경우 일반할당을 수행
	void* Allocate(int32 size);
	void	      Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// 0(1) 빠르게 찾기 위한 테이블
	// 메모리 크기와 메모리 풀을 빠르게 매핑하기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

// c++ 11 이전 문법에서는 인자마다 함수를 만들어줌.
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(Xalloc(sizeof(Type)));

	// placement new
	// 여기 메모리 위에다가 생성자를 호출해줘
	new(memory)Type(forward<Args>(args)...);
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	// 객체를 대상으로 소멸자를 호출해준 다음
	// 메모리를 반납하겠다
	obj->~Type();
	xrelease(obj);
}

template<typename Type>
shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>{ xnew<Type>(), xdelete<Type>};
}



