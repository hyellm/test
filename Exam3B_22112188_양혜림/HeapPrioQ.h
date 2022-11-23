/* -- HeapPrioQ.h -- */

#ifndef HEAP_PRIO_Q__H
#define HEAP_PRIO_Q__H

#include <iostream>
#include <fstream>
#include <mutex>
#include "CompleteBinaryTree.h"
using namespace std;

template<typename E>

class HeapPrioQueue : public CompleteBinaryTree<E>
{
public:
	HeapPrioQueue(int capa, string nm);  // 생성자
	~HeapPrioQueue();  // 소멸자
	bool isEmpty() { return size() == 0; }  // 비어있는지 확인하기
	bool isFull() { return size() == this->capacity; }  // 가득 차 있는지 확인하기
	int insert(E& elem);  // 삽입하기
	E* removeHeapMin();  // 우선순위 높은 값 제거하기
	E* getHeapMin();  // 우선순위 높은 값 추출하기
	void fprint(ofstream& fout);  // 파일에 출력하기
	int size() { return this->end; }  // 크기 반환하기

private:
	mutex mtx_heap_prioQ;
};

template<typename E>
HeapPrioQueue<E>::HeapPrioQueue(int capa, string nm)
	: CompleteBinaryTree<E>(capa, nm)
{
}

template<typename E>
HeapPrioQueue<E>::~HeapPrioQueue()
{ }

template<typename E>
void HeapPrioQueue<E>::fprint(ofstream& fout)
{
	if (size() <= 0)
	{
		fout << "HeapPriorityQueue is Empty !!" << endl;
		return;
	}
	else
		CompleteBinaryTree::fprintCBT(fout);
}

template<typename E>
int HeapPrioQueue<E>::insert(E& elem)
{
	int index, parent_index;
	E temp;

	if (isFull())
	{
		cout << this->getName() << " is Full !!" << endl;
		return size();
	}

	index = this->add_at_end(elem);

	/* up-heap bubbling */
	while (index != CBT_ROOT)
	{
		parent_index = this->parentIndex(index);

		if (this->t_array[index] >= this->t_array[parent_index])  // .getKey() 없앰
			break;
		else
		{
			//swap(this->t_GA[index], this->t_GA[parent_index]);
			temp = this->t_array[index];
			this->t_array[index] = this->t_array[parent_index];
			this->t_array[parent_index] = temp;
			index = parent_index;
		}
	}

	return size();
}

template<typename E>
E* HeapPrioQueue<E>::getHeapMin()
{
	E* pMinElem;
	if (size() <= 0)
	{
		return NULL;
	}
	pMinElem = (E*) new E;
	*pMinElem = this->getRootElement();

	return pMinElem;
}

template<typename E>
E* HeapPrioQueue<E>::removeHeapMin()
{
	int index_p, index_c, index_rc;
	E* pMinElem;
	E t_p, t_c, temp;
	int HPQ_size = size();

	if (HPQ_size <= 0)
		return NULL;

	pMinElem = (E*) new E;
	*pMinElem = this->getRootElement();

	if (HPQ_size == 1)
		this->removeCBTEnd();
	else
	{
		index_p = CBT_ROOT;
		this->t_array[CBT_ROOT] = this->t_array[this->end];
		this->end--;

		/* down-heap bubbling */
		while (this->hasLeftChild(index_p))
		{
			index_c = this->leftChildIndex(index_p);
			index_rc = this->rightChildIndex(index_p);
			if (this->hasRightChild(index_p) && (this->t_array[index_c] > this->t_array[index_rc]))
				index_c = index_rc;

			t_p = this->t_array[index_p];
			t_c = this->t_array[index_c];

			if (t_p > t_c)
			{
				//swap(this->t_GA[index_p], this->t_GA[index_c]);
				temp = this->t_array[index_p];
				this->t_array[index_p] = this->t_array[index_c];
				this->t_array[index_c] = temp;
				index_p = index_c;
			}
			else // (t_p <= t_c)
				break;
		} // end while
	}
	return pMinElem;
}

#endif // !HEAP_PRIO_Q__H
