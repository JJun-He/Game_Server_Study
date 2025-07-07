#pragma once


/*--------------------
:	 BaseAllocator
----------------------*/


class BaseAllocator
{
public:
	// 메모리를 할당하는 함수와 해제하는 함수를 만들어줌.
	static void*       Alloc(int32 size);
	static void        Release(void* ptr);
};

/*-------------------
:   StompAllocator
---------------------*/

// 버그를 찾는데 유용함
// 메모리가 필요 없어지면 확실하게 운영체제까지 부탁해 메모리를 완전히 날려버림
// -> 더 이상 접근 x
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

