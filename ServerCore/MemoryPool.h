#pragma once

// �޸� Ǯ�� ���� �� �Ҵ�
// [  ][  ][  ][  ][  ]

// [32]

// MemoryHeader�� �޸� ����� ��� ������ �����ϴ� ����ü�̴�.
// �ֿ� ������ �޸� ����� ũ��� ���� ��Ÿ�����͸� ����


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
	// �������� ���� ��ġ ��ȯ : �Ųٷ� �޸� ��� ����
	MemoryHeader(int32 size) : allocSize(size) {}

	// �־��� ����� �޸� ũ�⸦ �����ϰ�, ������ ������ ���� �ּҸ� ��ȯ
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	// ������ ������ �����͸� �޾Ƽ�, �ش� �޸� ����� ����� ��ȯ
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// TODO: �ʿ��� �߰� ����
};


/*---------------
	MemoryPool
-----------------*/
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	// �޸� Ǯ���� �Ҵ��� �޸� ����� ũ��
	MemoryPool(int32 allocSize);
	// �޸� Ǯ�� ����
	~MemoryPool();

	// �޸� ����� Ǯ�� ��ȯ
	void             Push(MemoryHeader* ptr);
	// �޸� Ǯ���� �޸� ����� ������
	MemoryHeader* Pop();

private:
	SLIST_HEADER   _header;
	int32          _allocSize = 0;
	// ���� �Ҵ�� �޸� ����� ����
	atomic<int32>  _useCount = 0;
	atomic<int32>  _reserveCount = 0;

	USE_LOCK;
	// ��� ������ �޸� ����� �����ϴ� ť
	queue<MemoryHeader*> _queue;
};

