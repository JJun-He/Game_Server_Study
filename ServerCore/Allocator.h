#pragma once


/*--------------------
:	 BaseAllocator
----------------------*/


class BaseAllocator
{
public:
	// �޸𸮸� �Ҵ��ϴ� �Լ��� �����ϴ� �Լ��� �������.
	static void*       Alloc(int32 size);
	static void        Release(void* ptr);
};

/*-------------------
:   StompAllocator
---------------------*/

// ���׸� ã�µ� ������
// �޸𸮰� �ʿ� �������� Ȯ���ϰ� �ü������ ��Ź�� �޸𸮸� ������ ��������
// -> �� �̻� ���� x
class StompAllocator
{
	enum { PAGE_SIZE = 0X1000 };

public:
	static void*    Alloc(int32 size);
	static void		Release(void* ptr);
};

/*-------------------
:   PoolAllocator
---------------------*/

class PoolAllocator
{
public:
	static void*    Alloc(int32 size);
	static void		Release(void* ptr);
};


/*-------------------
:   STL Allocator
---------------------*/

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator <Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(xalloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		xrelease(ptr);
	}

	template<typename U>
	struct rebind
	{
		using other = StlAllocator<U>;
	};
};

