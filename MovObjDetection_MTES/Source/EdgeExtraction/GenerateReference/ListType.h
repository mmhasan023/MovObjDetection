#ifndef _LISTHEADERTS_h
#define _LISTHEADERTS_h

//�ȱ��
//���� �ʿ����..
//����/����/��ġ�̵��� curNode��ȭ Ȯ��
//get() �Լ��� ������Ǽ� Ȯ��
//����Ʈ �׽�Ʈ ���α׷� ����
//prepend()�Լ� Ȯ��
//

#include <string.h>
#include <iostream>
using namespace std;

template <class Type> 
struct NodeType{
	Type  data;
	NodeType<Type> *link; 
	NodeType(Type a){data=a;};
};


//	������ �����Ͽ� �����ϴ� List class(List)�� ����. List classs ADT�� ������ ����. 
template <class Type> 
class ListType
{
public:
	ListType(); //������: first�� 0���� �ʱ�ȭ
	~ListType(); // ����� ��� ��带 �����Ѵ�. 
	void AddFront(Type item);	// ���� 2003.07.14 kellt -> item�� List�� �Ǿտ� �߰�
	void AddEnd(Type item);  // item�� List�� �ǵڿ� �߰�
	void ResetCurNode();  // curNode pointer�� ù ���� ����
	int GetNextItem(Type& data); // curNode�� ����Ű�� ����� �ڷḦ �����ϰ� curNode�� ���� ���� ��ũ. 
								 //����Ʈ end�� �����ϸ� null�� ����
	void PrintList(); // list�� ��� ������ ȭ�鿡 ����Ѵ�.

	long GetLength(void);
	Type get(int index);	//���� 2003.07.14 kellt-> n��° data ���� ����
	
	Type pop(void);
	void push(Type value);
	NodeType<Type> *next(void);
	Type value(void);
	void clear(void);
	
	bool prepend(ListType<Type>& l);//Adds the elements of l to the front of this list
	

private:
	NodeType<Type> *first; // ���Ḯ��Ʈ�� ������ ����Ų��
	NodeType<Type> *last; // ���Ḯ��Ʈ�� ���� ����Ų��
	NodeType<Type> *curNode; // ���� ��� ������

};



//	List(); //������: first�� 0���� �ʱ�ȭ
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

//---���� ���� 2003.07.14  pm1:45 kellt
template <class Type>
void ListType<Type>::AddFront(Type item)// list �տ� �߰� 
{
	//�� ��� ����
	NodeType<Type>* newNode;
	newNode = new NodeType<Type>(item);	// ��� �ʱ�ȭ
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
//---���� �� 2003.07.14  pm1:45 kellt

template <class Type> 
void ListType<Type>::AddEnd(Type item)  // item�� List�� �ǵڿ� �߰�
{
	//�� ��� ����
	NodeType<Type>* newNode;
	newNode = new NodeType<Type>(item);	// ��� �ʱ�ȭ
	if (newNode == NULL)
		return;

	newNode->link = NULL;
	// �� ����Ʈ�� ��쿡�� first�� �ʱ�ȭ 
	if (first == NULL)
		first = last = newNode;	
	else	//������ ��忡 ����
	{
		last->link = newNode;
		last = newNode;
	}
}

template <class Type> 
void ListType<Type> ::ResetCurNode()  // curNode pointer�� ù ���� ����
{
	curNode = first;
}

//iterator�� ���� �Լ�
// curNode�� ����Ű�� ����� �ڷḦ �����ϰ� curNode�� ���� ���� ��ũ. ����Ʈ end�� �����ϸ� null�� ����
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
void ListType<Type> :: PrintList() // list�� ��� ������ ȭ�鿡 ����Ѵ�.
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

//---�������� 2003.07.14 kellt-> n��° data ���� ����
template <class Type> 
Type ListType<Type>::get(int index)
{
//2003 0714 2032 ���� ����
//index=0�̰� GetLength() -1=0�� ��� ���� �ϳ� �������� 
//NULL�� ���� �Ǵ� ���� ����
//�ȱ��
//	if (GetLength() -1 <= index || index < 0)
	if (GetLength() -1 < index || index < 0)
		return NULL;
//2003 0714 2032 ���� ��

	NodeType<Type>* tmp = first;
	for (register int i = 0 ; tmp && i != index ; tmp = tmp->link, i++);
		return tmp->data;
}
//---���� �� 2003.07.14 kellt-> n��° data ���� ����

template <class Type> 
Type ListType<Type> :: pop(void)
{
	ResetCurNode();

	//����Ʈ ���������
	if (curNode == NULL)
		return NULL;    

	//����Ʈ�� �ϳ��� ������� ��
	if (curNode->link == NULL)
	{
		Type data = curNode->data;
		delete curNode;
		first = last = curNode = NULL;

		return data;
	}

	// ����Ʈ�� �ΰ� �̻� ������� ��
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

template <class T>
class Two_Stack
{
private:
	T m_Data[2];
	int cur;
public:
	Two_Stack(){cur=0;};
	~Two_Stack(){};

	void clear(){cur=0;};
	void push(T a)
	{
		if(cur>1)cur=1;
		m_Data[cur]=a;
		cur++;
	};
	T*   pop (   ){
		if(cur ){
			cur--;
			 return (m_Data+cur);}
		else return NULL;
	};
	int  GetCur(){return cur;};

};

#endif
