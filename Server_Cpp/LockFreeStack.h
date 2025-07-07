#pragma once

/*----------------
   1차 시도
-----------------*/

// 단순히 단일 연결 리스트(Singly Linked List)를 구현한 코드
// 스레드 안정성 고려 x

//struct SListEntry
//{
//	SListEntry* next;
//};
//
//class Data  // : public SListEntry
//{
//public:
//	SListEntry  _entry;
//
//	int32 _hp;
//	int32 _mp;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//// [data][ ][ ]
//// Header[ next ]
//
//// 리스트 헤더를 초기화
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

/*--------------------
      2차 시도
----------------------*/

//// 단일 연결리스트의 각 노드를 나타냄
//struct SListEntry
//{
//    // 다음 노드를 가리키는 포인터
//    SListEntry* next;
//};
//
//// 리스트의 헤더를 가리킴
//struct SListHeader
//{
//    // 리스트의 첫 번째 노드를 가리키는 포인터
//    SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

// --------------------
//     3차 시도
// --------------------

// 무조건 16바이트 정렬을 함
DECLSPEC_ALIGN(16)
// 단일 연결리스트의 각 노드를 나타냄
struct SListEntry
{
    // 다음 노드를 가리키는 포인터
    SListEntry* next;
};

// 리스트의 헤더를 가리킴
DECLSPEC_ALIGN(16)
struct SListHeader
{
    SListHeader()
    {
        alignment = 0;
        region = 0;
    }

    union
    {
        struct
        {
            uint64 alignment;
            uint64 region;
        }DUMMYSTRUCTNAME;

        struct
        {
            uint64 depth : 16;
            uint64 sequence : 48;
            uint64 reserved : 4;
            uint64 next : 60;
        }HeaderX64;
    };
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);