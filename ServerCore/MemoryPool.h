#pragma once

// 메모리 풀을 여러 개 할당
// [  ][  ][  ][  ][  ]

// [32]

// MemoryHeader는 메모리 블록의 헤더 정보를 저장하는 구조체이다.
// 주요 역할은 메모리 블록의 크기와 같은 메타데이터를 저장


enum
{
	SLIST_ALIGNMENT = 16
};


/*----------------
	MemoryHeader
-----------------*/

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	// [MemoryHeader][Data]
	// 데이터의 시작 위치 반환 : 거꾸로 메모리 헤더 추출
	MemoryHeader(int32 size) : allocSize(size) {}

	// 주어진 헤더에 메모리 크기를 설정하고, 데이터 영역의 시작 주소를 반환
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	// 데이터 영역의 포인터를 받아서, 해당 메모리 블록의 헤더를 반환
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// TODO: 필요한 추가 정보
};


/*---------------
	MemoryPool
-----------------*/
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	// 메모리 풀에서 할당할 메모리 블록의 크기
	MemoryPool(int32 allocSize);
	// 메모리 풀을 정리
	~MemoryPool();

	// 메모리 블록을 풀에 반환
	void             Push(MemoryHeader* ptr);
	// 메모리 풀에서 메모리 블록을 가져옴
	MemoryHeader* Pop();

private:
	SLIST_HEADER   _header;
	int32          _allocSize = 0;
	// 현재 할당된 메모리 블록의 개수
	atomic<int32>  _useCount = 0;
	atomic<int32>  _reserveCount = 0;

	USE_LOCK;
	// 사용 가능한 메모리 블록을 저장하는 큐
	queue<MemoryHeader*> _queue;
};

