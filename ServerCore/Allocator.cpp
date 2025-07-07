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

// 반환값 : 할당된 메모리의 시작 주소에 오프셋을 더한 값을 반환
void* StompAllocator::Alloc(int32 size)
{
	// 이 페이지 사이즈에 배수로 되어 있음
	// 끔찍한 메모리 오염을 방지할 수 있는 장치
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	// 좌표를 찍어줌(할당된 영역에서 실제로 사용할 공간을 빼줌) => 끝에 걸침
	const int64 dataOffset = pageCount * PAGE_SIZE - size;

	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	// 다른 타입이다 보니 캐스팅을 해서 포인터 계산
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

// 기능 : 주어진 포인터가 가리키는 메모리를 페이지 단위로 해제
// 구현 : VirtualFree 함수를 사용
void StompAllocator::Release(void* ptr)
{
	// 포인터 주소를 정수로 다루기 위함
	const int64 address = reinterpret_cast<int64>(ptr);
	// ptr이 속한 페이지의 시작 주소를 개선 => 이는 페이지 경계에 맞춰 메모리를 해제하기 위함
	const int64 baseAddress = address - (address % PAGE_SIZE);
	// baseAddress가 가리키는 페이지를 해제 => MEM_RELEASE 플래그는 전체 메모리 범위를 해제
	// 주의사항: VirtualFree 함수는 VirtualAlloc으로 할당된 메모리를 해제하는데 사용
	// ptr이 VirtualAlloc으로 할당된 메모리가 아닌 경우, 동작이 정의되지 않을 수 있음
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
