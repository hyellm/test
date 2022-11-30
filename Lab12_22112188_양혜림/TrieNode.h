/* -- TrieNode.h -- */

#ifndef TRIENODE__H
#define TRIENODE__H

#include <iostream>
#include <string>
#include <list>
#define VALUE_INTERNAL_NODE NULL
using namespace std;

typedef list<string> STL_list;

template <typename E>
class TrieNode
{
public:
	TrieNode() {} // default constructor
	TrieNode(char k, E v) : key(k), value(v) { prev = next = parent = child = NULL; }
	void setKey(char k) { key = k; }
	void setValue(E v) { value = v; }
	void setNext(TrieNode<E>* nxt) { next = nxt; }
	void setPrev(TrieNode<E>* pv) { prev = pv; }
	void setParent(TrieNode<E>* pr) { parent = pr; }
	void setChild(TrieNode<E>* chld) { child = chld; }
	char getKey() { return key; }
	E getValue() { return value; }
	TrieNode<E>* getPrev() { return prev; }
	TrieNode<E>* getNext() { return next; }
	TrieNode<E>* getParent() { return parent; }
	TrieNode<E>* getChild() { return child; }
	void _fprint(ostream& fout, TrieNode<E>* pTN, int indent);
private:
	char key;
	E value;
	TrieNode<E>* prev;
	TrieNode<E>* next;
	TrieNode<E>* parent;
	TrieNode<E>* child;
};

template<typename E>
void TrieNode<E>::_fprint(ostream& fout, TrieNode<E>* pTN, int indent)
{
	if (pTN == NULL)
	{
		fout << endl;
		return;
	}

	else
	{
		fout << pTN->key; _fprint(fout, pTN->child, indent + 1);

		if (pTN->next == NULL)
			return;

		for (int i = 0; i < indent; i++)
			fout << " ";

		_fprint(fout, pTN->next, indent);
	}
}

#endif // !TRIENODE__H
