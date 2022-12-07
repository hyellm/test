/* -- Trie.h -- */

#ifndef TRIE__H
#define TRIE__H

#include <iostream>
#include <string>
#include "TrieNode.h"
#define MAX_STR_LEN 50
//#define DEBUG
using namespace std;

typedef list<MyVoca*> List_pVoca;
typedef list<MyVoca*>::iterator List_pVoca_Iter;

enum SearchMode { FULL_MATCH, PREFIX_MATCH };

template <typename E>
class Trie
{
public:
	Trie(string name); // constructor
	int size() { return num_keys; }
	void insert(string keyStr, E value);
	void insertExternalTN(TrieNode<E>* pTN, string keyStr, E value);
	TrieNode<E>* find(string keyStr);
	void findPrefixMatch(string prefix, List_pVoca& predictWords);
	void deleteKeyStr(string keyStr);
	void eraseTrie();
	void fprintTrie(ostream& fout);

protected:
	TrieNode<E>* _find(string keyStr, SearchMode sm = FULL_MATCH);
	void _traverse(TrieNode<E>* pTN, List_pVoca& list_keywords);

private:
	TrieNode<E>* _root; // _root trie node
	int num_keys;
	string trie_name;
};

template<typename E>
Trie<E>::Trie(string name)
{
	trie_name = name;

	_root = new TrieNode<E>('\0', NULL);
	_root->setKey('\0');
	_root->setPrev(NULL);
	_root->setNext(NULL);
	_root->setParent(NULL);
	_root->setChild(NULL);

	num_keys = 0;
}

template<typename E>
void Trie<E>::insertExternalTN(TrieNode<E>* pTN, string keyStr, E value)
{
	TrieNode<E>* pTN_New = NULL;
	pTN_New = new TrieNode<E>('\0', value);

	pTN->setChild(pTN_New);
	(pTN->getChild())->setParent(pTN);
	pTN_New->setValue(value);
	//cout << "key (" << keyStr << ") is inserted \n";
}

template<typename E>
void Trie<E>::insert(string keyStr, E value)
{
	TrieNode<E>* pTN = NULL, * pTN_New = NULL;

	char* keyPtr = (char*)keyStr.c_str();

	if (keyPtr == NULL)
		return;

	/* Firstly, check any possible duplicated key insertion */
	if (_find(keyStr, FULL_MATCH) != NULL)
	{
		cout << "The given key string (" << keyStr << ") is already existing; just return !!" << endl;
		return;
	}

	pTN = this->_root;

	while ((pTN != NULL) && (*keyPtr != '\0'))
	{
		if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))
			break;

		while ((pTN->getKey() < *keyPtr) && (pTN->getNext() != NULL))
			pTN = pTN->getNext();

		while ((pTN != NULL) && (pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))
		{
			pTN = pTN->getChild();
			keyPtr++;
		}

		if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))
			break;
	} // end while for positioning

	/* Secondly, the given key string is a sub-string of an existing key */
	/* e.g.) trying to insert "abc" while "abcde" is already exisiting. */
	if ((pTN->getKey() != '\0') && (*keyPtr == '\0'))
	{
		/* there was a longer key string already !! */
		/* break the longer key string, and connected to the separated key strings */
		pTN_New = new TrieNode<E>('\0', value);
		pTN_New->setParent(pTN->getParent());
		(pTN->getParent())->setChild(pTN_New);
		pTN_New->setNext(pTN);
		pTN->setPrev(pTN_New);

		//cout << "key (" << keyWord << ") is inserted" << endl;
		this->num_keys++;
		return;
	}
	else if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))
	{
		/* at this level, a new substring is inserted as the last nodes */
		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);
		pTN_New->setParent(pTN->getParent());
		pTN_New->setPrev(pTN);
		pTN->setNext(pTN_New);

		pTN = pTN_New;
		keyPtr++;

		while (*keyPtr != '\0')
		{
			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);
			pTN->setChild(pTN_New);
			(pTN->getChild())->setParent(pTN);
			pTN = pTN->getChild();

			keyPtr++;
		}

		if (*keyPtr == '\0')
		{
			insertExternalTN(pTN, keyStr, value);
			this->num_keys++;
			return;
		}
	}
	else if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0')) {
		/* insert between two existing trie nodes */
		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);
		pTN_New->setNext(pTN);
		pTN_New->setParent(pTN->getParent());

		if (pTN->getPrev() == NULL)
		{ /* this pTN_new becomes the new first in this level */
			if (pTN->getParent() != NULL)
				(pTN->getParent())->setChild(pTN_New);
		}

		else
			(pTN->getPrev())->setNext(pTN_New);

		pTN_New->setPrev(pTN->getPrev());
		pTN->setPrev(pTN_New);
		pTN = pTN_New;
		keyPtr++;

		while (*keyPtr != '\0')
		{
			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);
			pTN->setChild(pTN_New);
			(pTN->getChild())->setParent(pTN);
			pTN = pTN->getChild();
			keyPtr++;
		}

		if (*keyPtr == '\0')
		{
			insertExternalTN(pTN, keyStr, value);
			this->num_keys++;
			return;
		}
	}
}

template<typename E>
TrieNode<E>* Trie<E>::find(string keyStr)
{
	TrieNode<E>* pTN = NULL;
	pTN = _find(keyStr, FULL_MATCH);
	return pTN;
}

template<typename E>
TrieNode<E>* Trie<E>::_find(string keyStr, SearchMode sm)
{
	const char* keyPtr;
	TrieNode<E>* pTN = NULL;
	TrieNode<E>* found = NULL;

	keyPtr = (char*)keyStr.c_str();

	// 기존 : if (keyStr == NULL)
	if (keyPtr == NULL)
		return NULL;

	pTN = this->_root;

	while ((pTN != NULL) && (*keyPtr != '\0'))
	{
		while ((pTN != NULL) && (pTN->getKey() < *keyPtr))
		{
			if (pTN->getNext() == NULL)
				return NULL;
			pTN = pTN->getNext();
		}

		if ((pTN != NULL) && (pTN->getKey() > *keyPtr))
		{
			// key not found
			return NULL;
		}

		else if ((pTN == NULL) && (*keyPtr != '\0'))
		{
			// key not found
			return NULL;
		}

		else if ((pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))
		{
			pTN = pTN->getChild();
			keyPtr++;
			if (*keyPtr == '\0')
			{
				/* key or prefix found */
				if (sm == FULL_MATCH)
				{
					if (pTN->getKey() == '\0')
					{
						/* found the key string as a full-match */
						return pTN;
					}
					else // (pTN->getKey() != '\0')
					{
						/* found the key string as a substring of a longer existing string */
						return NULL;
					}
				}
				else if (sm == PREFIX_MATCH)
				{
					/* found the key string as a full-match or as a substring of a longer existing string */
					return pTN;
				}
			}
			else if ((pTN->getKey() == '\0') && (*keyPtr != '\0'))
			{
				if (pTN->getNext() != NULL)
				{
					pTN = pTN->getNext(); continue;
				}
				else
					return NULL;
			}
			else
			{
				continue;
			}
		}
	} // end while
}

template<typename E>
void Trie<E>::_traverse(TrieNode<E>* pTN, List_pVoca& list_keywords)
{
	if (pTN == NULL)
		return;
	if (pTN->getChild() == NULL)
	{
		list_keywords.push_back(pTN->getValue());
	}
	else
	{
		_traverse(pTN->getChild(), list_keywords);
	}
	if (pTN->getNext() != NULL)
	{
		_traverse(pTN->getNext(), list_keywords);
	}
}

template<typename E>
void Trie<E>::findPrefixMatch(string keyStr, List_pVoca& predictWords)
{
	TrieNode<E>* pPtr = NULL;
	const char* keyPtr;
	TrieNode<E>* pTN = NULL;
	TrieNode<E>* found = NULL;
	keyPtr = keyStr.c_str(); // .c_str()

	// 기존: if (keyStr == NULL)
	if (keyPtr == NULL)
		return;

	pTN = this->_root;
	pTN = _find(keyStr, PREFIX_MATCH);

	_traverse(pTN, predictWords);
	//printf("Error in TrieSearch (key: %s) !!\n", keyWord);
}

template<typename E>
void Trie<E>::deleteKeyStr(string keyStr)
{
	TrieNode<E>* pTN = NULL, * _root;
	TrieNode<E>* tmp = NULL;
	int trie_val;
	_root = this->_root;


	// 기존: if (NULL == _root || NULL == keyStr)
	if (NULL == _root || NULL == keyStr.c_str())
		return;

	pTN = _find(keyStr, FULL_MATCH);
	if (pTN == NULL) {
		cout << "Key [" << keyStr << "] not found in trie" << endl;
		return;
	}
	while (1) {
		if (pTN == NULL)
			break;
		if (pTN->getPrev() && pTN->getNext()) {
			tmp = pTN; (pTN->getNext())->setPrev(pTN->getPrev()); (pTN->getPrev())->setNext(pTN->getNext());
			free(tmp);
			break;
		}
		else if (pTN->getPrev() && !(pTN->getNext()))
		{
			tmp = pTN;
			(pTN->getPrev())->setNext(NULL);
			free(tmp);
			break;
		}
		else if (!(pTN->getPrev()) && pTN->getNext())
		{
			tmp = pTN;
			(pTN->getParent())->setChild(pTN->getNext());
			pTN = pTN->getNext(); pTN->setPrev(NULL);
			free(tmp); break;
		}
		else
		{
			tmp = pTN;
			pTN = pTN->getParent();
			if (pTN != NULL) pTN->setChild(NULL);
			free(tmp);
			if ((pTN == _root) && (pTN->getNext() == NULL) && (pTN->getPrev() == NULL))
			{
				cout << "Now, the trie is empty !!" << endl;
				break;
			}
		}
	}
	this->num_keys--;
}

template<typename E>
void Trie<E>::eraseTrie()
{
	TrieNode<E>* pTN;
	TrieNode<E>* pTN_to_be_deleted = NULL;

	if (this->_root == NULL)
		return;

	pTN = this->_root;

	/* delete the last key word first */
	while (pTN != NULL) {
		while ((pTN != NULL) && (pTN->getNext())) pTN = pTN->getNext(); while (pTN->getChild()) {
			if (pTN->getNext()) break;
			pTN = pTN->getChild();
		}
		if (pTN->getNext()) continue;
		if (pTN->getPrev() && pTN->getNext()) {
			pTN_to_be_deleted = pTN; (pTN->getNext())->setPrev(pTN->getPrev()); (pTN->getPrev())->setNext(pTN->getNext()); pTN = pTN->getNext();
			free(pTN_to_be_deleted);
		}
		else if (pTN->getPrev() && !(pTN->getNext()))
		{
			pTN_to_be_deleted = pTN;
			(pTN->getPrev())->setNext(NULL);
			pTN = pTN->getPrev();
			free(pTN_to_be_deleted);
		}
		else if (!(pTN->getPrev()) && pTN->getNext())
		{
			pTN_to_be_deleted = pTN;
			(pTN->getParent())->setChild(pTN->getNext());
			(pTN->getNext())->setPrev(NULL);
			pTN = pTN->getNext();
			free(pTN_to_be_deleted);
		}
		else
		{
			pTN_to_be_deleted = pTN;
			if (pTN == this->_root)
			{
				/* _root */
				this->num_keys = 0;
				return;
			}
			if (pTN->getParent() != NULL)
			{
				pTN = pTN->getParent();
				pTN->setChild(NULL);
			}
			else
			{
				pTN = pTN->getPrev();
			}
			free(pTN_to_be_deleted);
		} // end if - else
	} // end while
}

//template<typename E>
//void TrieNode<E>::_fprint(ostream& fout, TrieNode<E>* pTN, int indent)
//{
//	if (pTN == NULL)
//	{
//		fout << endl;
//		return;
//	}
//
//	else
//	{
//		fout << pTN->key; _fprint(fout, pTN->child, indent + 1);
//
//		if (pTN->next == NULL)
//			return;
//
//		for (int i = 0; i < indent; i++)
//			fout << " "; _fprint(fout, pTN->next, indent);
//	}
//}

template<typename E>
void Trie<E>::fprintTrie(ostream& fout)
{
	TrieNode<E>* pTN;
	int line = 1, indent = 0;

	fout << "trie ( " << this->trie_name << ") with " << this->num_keys << " trie_nodes\n";

	if (this->num_keys == 0)
	{
		fout << "Empty trie !" << endl;
		return;
	}

	pTN = this->_root;
	pTN->_fprint(fout, pTN, indent);
}

#endif // !TRIE__H