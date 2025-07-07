#pragma once
#include "Types.h"
#include "Allocator.h"
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
using namespace std;

template<typename Type>
using Vector = vector<Type, StlAllocator<Type>>;

template<typename Type>
using List = list<Type, StlAllocator<Type>>;

// map(키(key)와 값(value)을 쌍으로 저장하는 자료 구조)
// map은 각 요소가 std::pair 객체로 저장
// 중복되지 않는 키: 각 키는 고유해야 하며, 중복된 키는 허용 x.
// 자동 정렬(키를 기준으로 자동으로 정렬)
// 노드 기반의 균형 이진 트리: 내부적으로 균형 이진 트리(예: 레드-블랙 트리) 구조로 구현.
template<typename Key, typename Type, typename Pred = less<Key>>
using Map = map<Key, Type, Pred, StlAllocator<pair<const Key, Type>>>;

// Set은 고유한 값을 저장하며 자동으로 정렬하는 자료 구조
// - 고유한 값 저장 : set은 동일한 값을 중복하여 저장 x, 각 요소는 유일
// - 자동 정렬 : 요소들은 기본적으로 오름차순으로 정렬, 사용자 정의 비교 함수를 통해 정렬 기준을 변경
template<typename Key, typename Pred = less<Key>>
using Set = set<Key, Pred, StlAllocator<Key>>;

// 양쪽 끝에서 요소의 삽입과 삭제가 모두 가능한 선형 자료구조
// 이는 큐(Queue)와 스택의 특성
// - 양방향 삽입 및 삭제: Deque는 앞(front)과 뒤(rear) 양쪽 끝에서 요소를 추가 or 제거
// - 큐와 스택의 특성 : Deque를 이용하면 FIFO, LIFO 모두 구현
template<typename Type>
using Deque = deque<Type, StlAllocator<Type>>;

template<typename Type, typename Container = Deque<Type>>
using Queue = queue<Type, Container>;

template<typename Type, typename Container = Deque<Type>>
using Stack = stack<Type, Container>;

template<typename Type, typename Container = Vector<Type>, typename Pred = less<typename Container::value_type>>
using PriorityQueue = priority_queue<Type, Container, Pred>;

// basic_string은 3가지 템플릿 매개변수를 가짐
// CharT : 문자열을 구성하는 문자 타입
// Traits : 문자 특성을 정의하는 클래스
// Allocator : 메모리 할당을 위한 할당자 클래스 
using String = basic_string<char, char_traits<char>, StlAllocator<char>>;

using WString = basic_string<wchar_t, char_traits<wchar_t>, StlAllocator<wchar_t>>;

template<typename Key, typename Type, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashMap = unordered_map<Key, Type, Hasher, KeyEq, StlAllocator<pair<const Key, Type>>>;

template<typename Key, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashSet = unordered_set<Key, Hasher, KeyEq, StlAllocator<Key>>;