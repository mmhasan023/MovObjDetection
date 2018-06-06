/*
#include "ListType.h"
//안기옥
//수정 필요사항..
//삽입/삭제/위치이동시 curNode변화 확인
//get() 함수의 모든경우의수 확인
//리스트 테스트 프로그램 구현
//prepend()함수 확인
//
//	List(); //생성자: first를 0으로 초기화
template <class Type> 
ListType<Type> ::ListType()
{
	first=last=curNode=NULL;
}

template <class Type> 
ListType<Type> ::~ListType()
{
	NodeType<Type> *tmp, *p;
	for (p=first; p!=NULL; )
	{
		tmp = p;
		p = p->link;
		delete tmp;
	}
}

//---수정 시작 2003.07.14  pm1:45 kellt
template <class Type>
void ListType<Type>::AddFront(Type item)// list 앞에 추가 
{
	//새 노드 생성
	NodeType<Type>* newNode;
	newNode = new NodeType<Type>(item);	// 노드 초기화
	if (newNode == NULL)
		return;

	newNode->link = NULL;
	if (!first) // empty list
	{
		first = last = newNode;
	}
	else
	{
		newNode->link = first;
		first = newNode;
	}
}
//---수정 끝 2003.07.14  pm1:45 kellt

template <class Type> 
void ListType<Type>::AddEnd(Type item)  // item을 List의 맨뒤에 추가
{
	//새 노드 생성
	NodeType<Type>* newNode;
	newNode = new NodeType<Type>(item);	// 노드 초기화
	if (newNode == NULL)
		return;

	newNode->link = NULL;
	// 빈 리스트인 경우에는 first를 초기화 
	if (first == NULL)
		first = last = newNode;	
	else	//마지막 노드에 연결
	{
		last->link = newNode;
		last = newNode;
	}
}

template <class Type> 
void ListType<Type> ::ResetCurNode()  // curNode pointer를 첫 노드로 세팅
{
	curNode = first;
}

//iterator을 위한 함수
// curNode가 가리키는 노드의 자료를 리턴하고 curNode를 다음 노드로 링크. 리스트 end에 도달하면 null을 리턴
template <class Type> 
int ListType<Type> :: GetNextItem(Type& data) 
{
	if (curNode==NULL) return 0;
	else 
	{
		data = curNode->data;
		curNode = curNode->link;
		return 1;
	}
}

template <class Type> 
void ListType<Type> :: PrintList() // list의 모든 노드들을 화면에 출력한다.
{
	Type data;
	ResetCurNode();
	while (GetNextItem(data))
		cout<< data << "--> ";
	cout<<endl;
}

template <class Type> 
long ListType<Type> :: GetLength(void)
{
	long length = 0;
	NodeType<Type>* tmp = 0;
	tmp = first;
	for ( ; tmp ; tmp = tmp->link, length++);
		return length;
}

//---수정시작 2003.07.14 kellt-> n번째 data 갖고 오기
template <class Type> 
Type ListType<Type>::get(int index)
{
//2003 0714 2032 수정 시작
//index=0이고 GetLength() -1=0일 경우 값이 하나 있음에도 
//NULL이 리턴 되는 문제 수정
//안기옥
//	if (GetLength() -1 <= index || index < 0)
	if (GetLength() -1 < index || index < 0)
		return NULL;
//2003 0714 2032 수정 끝

	NodeType<Type>* tmp = first;
	for (register int i = 0 ; tmp && i != index ; tmp = tmp->link, i++);
		return tmp->data;
}
//---수정 끝 2003.07.14 kellt-> n번째 data 갖고 오기

template <class Type> 
Type ListType<Type> :: pop(void)
{
	ResetCurNode();

	//리스트 비어있을때
	if (curNode == NULL)
		return NULL;    

	//리스트에 하나만 들어있을 때
	if (curNode->link == NULL)
	{
		Type data = curNode->data;
		delete curNode;
		first = last = curNode = NULL;

		return data;
	}

	// 리스트에 두개 이상 들어있을 때
	for ( ; curNode->link != NULL ; curNode = curNode->link)
		if (curNode->link->link == NULL)
			break;

	NodeType<Type> *tmp = curNode->link;
	Type data = tmp->data;
	delete tmp;

	curNode->link = NULL;
	last = curNode;

	return data;
}

template <class Type> 
void ListType<Type> :: push(Type value)
{
	AddEnd(value);
}

template <class Type> 
NodeType<Type> *ListType<Type> :: next(void)
{
	if (curNode == NULL)
		return curNode;

	if (curNode->link != NULL)
		curNode = curNode->link;
	else
		curNode = NULL;

	return curNode;
}

template <class Type> 
Type ListType<Type> :: value(void)
{
	return curNode->data;
}

template <class Type> 
void ListType<Type> :: clear(void)
{
	if (first == NULL)
		return;

	NodeType<Type> *tmp;
	for (NodeType<Type> *p = first ; p != NULL ; )
	{
		tmp = p;
		p = p->link;
		delete tmp;
	}

	first = last = curNode = NULL;
}

// prepend() -- Adds the elements of l to the front of this list. O(n).
// Returns true if new nodes are created. Current position is set to
// the first element added.
template <class Type> 
bool ListType<Type> :: prepend(ListType<Type>& l)
{
	int len = l.GetLength();

	for (register int i = 0 ; i < len ; i++)
		AddFront(l.get(len - 1 - i));

	return true;
}

*/