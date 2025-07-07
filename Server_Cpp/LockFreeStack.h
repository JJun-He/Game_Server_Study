#pragma once

/*----------------
   1�� �õ�
-----------------*/

// �ܼ��� ���� ���� ����Ʈ(Singly Linked List)�� ������ �ڵ�
// ������ ������ ��� x

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
//// ����Ʈ ����� �ʱ�ȭ
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

/*--------------------
      2�� �õ�
----------------------*/

//// ���� ���Ḯ��Ʈ�� �� ��带 ��Ÿ��
//struct SListEntry
//{
//    // ���� ��带 ����Ű�� ������
//    SListEntry* next;
//};
//
//// ����Ʈ�� ����� ����Ŵ
//struct SListHeader
//{
//    // ����Ʈ�� ù ��° ��带 ����Ű�� ������
//    SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

// --------------------
//     3�� �õ�
// --------------------

// ������ 16����Ʈ ������ ��
DECLSPEC_ALIGN(16)
// ���� ���Ḯ��Ʈ�� �� ��带 ��Ÿ��
struct SListEntry
{
    // ���� ��带 ����Ű�� ������
    SListEntry* next;
};

// ����Ʈ�� ����� ����Ŵ
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