#pragma once
#include "Allocator.h"
#include "MemoryPool.h"

class MememoryPool;

/*-----------------
	Memory
------------------*/

// Memory Ŭ������ �޸� Ǯ�� �����ϰ�, �޸� �Ҵ�� ������ ó���ϴ� ����
class Memory
{
	enum
	{
		// ~1024���� 32����, ~2048���� 128����, ~4096���� 256����
		POOL_COUNT = (1024 / 32) * (1024 / 128) * (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	// �־��� ũ���� �޸𸮸� �Ҵ�
	// �޸� Ǯ���� �޸𸮸� �������ų� �ִ� ũ�⸦ �ʰ��ϴ� ��� �Ϲ��Ҵ��� ����
	void* Allocate(int32 size);
	void	      Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// �޸� ũ�� <-> �޸� Ǯ
	// 0(1) ������ ã�� ���� ���̺�
	// �޸� ũ��� �޸� Ǯ�� ������ �����ϱ� ���� ���̺�
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

// c++ 11 ���� ���������� ���ڸ��� �Լ��� �������.
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(Xalloc(sizeof(Type)));

	// placement new
	// ���� �޸� �����ٰ� �����ڸ� ȣ������
	new(memory)Type(forward<Args>(args)...);
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	// ��ü�� ������� �Ҹ��ڸ� ȣ������ ����
	// �޸𸮸� �ݳ��ϰڴ�
	obj->~Type();
	xrelease(obj);
}

template<typename Type>
shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>{ xnew<Type>(), xdelete<Type>};
}



