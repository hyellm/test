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

	_root = new TrieNode<E>('\0', "");
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

	//keyPtr = keyStr;

	// ���� : if (keyStr == NULL)
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

	// ����: if (keyStr == NULL)
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

	
	// ����: if (NULL == _root || NULL == keyStr)
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

// =============================================================================================================
// ������ ���� ��

//#ifndef TR_H
//#define TR_H
//
//#include"TrieNode.h"
//#include"MyVoca.h"
//
//typedef list<MyVoca*>List_pVoca;
//typedef list<MyVoca*>::iterator List_pVoca_Iter;
//
//enum SearchMode { FULL_MATCH, PREFIX_MATCH };
//
//template <typename E>
//class Trie
//{
//public:
//	Trie(string name);//������
//	int size() { return num_keys; }//ũ�� ��ȯ
//	void insert(string keyStr, E value);//����
//	void insertExternalTN(TrieNode<E>* pTN, string keyStr, E value);//External��� ����
//	TrieNode<E>* find(string keyStr);//Ű���� Ž��
//	void findPrefixMatch(string prefix, List_pVoca& predictVocas);//������ ���ξ� ���� �ܾ����
//	void deleteKeyStr(string keyStr);//Ű���带 ������� ����
//	void eraseTrie();//��ü ����
//	void fprintTrie(ostream& fout);//���
//protected:
//	TrieNode<E>* _find(string keyStr, SearchMode sm = FULL_MATCH);//Ű������ġ�� ��� ��ȯ ��ġ��忡 ���� 
//	void _traverse(TrieNode<E>* pTN, List_pVoca& list_keywords);//Ű���� ����Ʈ�� �����
//private:
//	TrieNode<E>* _root;//��Ʈ���
//	int num_keys;//Ű�� ��
//	string trie_name;//�̸�
//};
//
//template<typename E>
//Trie<E>::Trie(string name)
//{
//	trie_name = name;//�̸�����
//	_root = new TrieNode<E>(' ', NULL);//��Ʈ��� ����
//	_root->setKey(' ');//Ű ����
//	_root->setPrev(NULL);//���� �ʱ�ȭ
//	_root->setNext(NULL);//���� �ʱ�ȭ
//	_root->setParent(NULL);//�θ� �ʱ�ȭ
//	_root->setChild(NULL);//�ڽ� �ʱ�ȭ
//	num_keys = 0;
//}
//
//
//template<typename E>
//void Trie<E>::insertExternalTN(TrieNode<E>* pTN, string keyStr, E value)
//{
//	TrieNode<E>* pTN_New = NULL;
//
//	pTN_New = new TrieNode<E>('\0', value);//external��� ����
//	pTN->setChild(pTN_New);//pTN�� �ڽ����� ����
//	(pTN->getChild())->setParent(pTN);//pTN_New �θ�� pTN
//	pTN_New->setValue(value);//�� ����
//}
//
//template<typename E>
//void Trie<E>::insert(string keyStr, E value)
//{
//	TrieNode<E>* pTN = NULL, * pTN_New = NULL;
//	char* keyPtr = (char*)keyStr.c_str();//�ܾ ���縵 �ϳ��ϳ��� ����
//
//	if (keyStr == "")
//		return;
//	if (_find(keyStr, FULL_MATCH) != NULL)
//	{
//		cout << "The given key string (" << keyStr << ") is already existing; just return !!" << endl;//Ű���尡 �̹� ������ ���
//		return;
//	}
//
//	pTN = this->_root;//��Ʈ���� ����
//
//	while ((pTN != NULL) && (*keyPtr != '\0'))//pTN�� NULL�� �ƴϰ� �����Ϸ��� Ű�� ������ ���
//	{
//		if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))//������ġ�� Ű���� ���� Ű������ ũ�� �ٷ� ������ġ�� NULL�ϰ�� 
//			break;//�ٷ� ������ �����ϸ� �Ǳ⶧���� �극��ũ
//		while ((pTN->getKey() < *keyPtr) && (pTN->getNext() != NULL))//pTN ���� ��ġ�� ��尡 �̹� ������ ���
//		{
//			pTN = pTN->getNext();//�������� �Ѿ�� ������ NULL�� �ɶ����� ����
//		}
//		while ((pTN != NULL) && (pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))//Ű�� ������ ���
//		{
//			pTN = pTN->getChild();//�ڽĳ��� �̵�
//			keyPtr++;
//		}
//		if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))//pTN�տ� ��ġ�� �����ϴ� ���
//			break;
//	}
//
//	//�����ҷ��� Ű���尡 ����Ű���忡 ���ԵǾ��ִ°��
//	if ((pTN->getKey() != '\0') && (*keyPtr == '\0'))//���� ����� Ű�� �� ����Ű���带 �����ϸ鼭 ���縵�� �����ְ� �����ҷ��� �ܾ��� ���縵�� �������ΰ��
//	{
//		//���� �����ҷ��¹��ڸ� �����ϴ� �� �乮�� ����
//		pTN_New = new TrieNode<E>('\0', value);//��� ����
//		pTN_New->setParent(pTN->getParent());//��������� �θ� �θ���� ����
//		(pTN->getParent())->setChild(pTN_New);//���� �θ����� �ڽĳ�带 ���Ӱ� ������ ���μ���
//		pTN_New->setNext(pTN);//�����ҷ��� Ű�� ����������� �������� ������ ������������ ����
//		pTN->setPrev(pTN_New);//�������� �����ؼ� ����
//		this->num_keys++;//Ű�� ����
//
//		return;
//	}
//	else if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))//�ٷ� �����ʿ� ���԰����Ͽ� �ܾ ����
//	{
//
//		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//Ű���带 ���� ������
//		pTN_New->setParent(pTN->getParent());//�θ���� ����
//		pTN_New->setPrev(pTN);//������ pTN
//		pTN->setNext(pTN_New);//���ĸ� pTN_New ���� ����
//		pTN = pTN_New;//�������� �̵�
//		keyPtr++;
//		while (*keyPtr != '\0')//���ڰ� �����Ұ��
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//��� ����
//			pTN->setChild(pTN_New);//�ڽ����� ����
//			(pTN->getChild())->setParent(pTN);//�θ�� ���� ���� ����
//			pTN = pTN->getChild();//�Ʒ���ġ�� ������
//			keyPtr++;
//		}
//		if (*keyPtr == '\0')//���ڰ� ������
//		{
//			insertExternalTN(pTN, keyStr, value);//������ ��� �߰�
//			this->num_keys++;
//			return;
//		}
//	}
//
//	else if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))//pTN��ġ���� �տ� ���ԵǴ°��
//	{
//
//		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//��� ����
//		pTN_New->setNext(pTN);//pTN���� �������� ����
//		pTN_New->setParent(pTN->getParent());//�θ���� ����
//		if (pTN->getPrev() == NULL)//������ NULL�̾��ٸ� �������� ù��° �����
//		{
//			if (pTN->getParent() != NULL)//�θ� �ִٸ�
//				(pTN->getParent())->setChild(pTN_New);//�θ� �ڽ� ����
//		}
//		else
//		{
//			(pTN->getPrev())->setNext(pTN_New);//pTN�� ��������� �������� ����
//		}
//		pTN_New->setPrev(pTN->getPrev());//���λ����� ����� ������ ��������� ����
//		pTN->setPrev(pTN_New);//pTN������ ���� ������ ���
//		pTN = pTN_New;//�������� �̵�
//		keyPtr++;//���� ����ó���� ����
//		while (*keyPtr != '\0')//���ڰ� �������
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//��� ����
//			pTN->setChild(pTN_New);//�ڽ� ����
//			(pTN->getChild())->setParent(pTN);//�θ��ڽ� ����
//			pTN = pTN->getChild();//�Ʒ��� �̵�
//			keyPtr++;//���� ����ó���� ����
//		}
//		if (*keyPtr == '\0')//���ڸ� ��ó���������
//		{
//			insertExternalTN(pTN, keyStr, value);//External��� �߰�
//			this->num_keys++;
//			return;
//		}
//	}
//}
//
//
//template<typename E>
//TrieNode<E>* Trie<E>::find(string keyStr)
//{
//	TrieNode<E>* pTN = NULL;//��� ����
//	pTN = _find(keyStr, FULL_MATCH);//Ű����� ������ ��ġ�ϴ� ���
//	return pTN;
//}
//
//template<typename E>
//TrieNode<E>* Trie<E>::_find(string keyStr, SearchMode sm)
//{
//	const char* keyPtr = (char*)keyStr.c_str();
//	TrieNode<E>* pTN = NULL;//��� ����
//	TrieNode<E>* found = NULL;//ã�� ���
//	if (keyPtr == NULL)//ã��Ű�� ������
//		return NULL;
//
//	pTN = this->_root;//��Ʈ���� ����
//	while ((pTN != NULL) && (*keyPtr != '\0'))//��尡 �����ϰ� ���ڰ� �����ϸ�
//	{
//		while ((pTN != NULL) && (pTN->getKey() < *keyPtr))//ã������ ������ Ű���� ��ū���
//		{
//			if (pTN->getNext() == NULL)//������ NULL�̸�
//				return NULL;//���ڰ� �������� ����
//			pTN = pTN->getNext();//�������� �̵�
//		}
//		if ((pTN != NULL) && (pTN->getKey() > *keyPtr))//ã������ ������ Ű�� ��ū���(�̹� �������� �̵��߱⿡)
//		{
//			return NULL;//ã������
//		}
//		else if ((pTN == NULL) && (*keyPtr != '\0'))//���ڴ� �����ϳ� �̵��� ��尡����
//		{
//			return NULL;//ã������
//		}
//		else if ((pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))//���ڰ� �����ϸ� ���ڰ� �����ִ°��
//		{
//			pTN = pTN->getChild();//�Ʒ��� ����
//			keyPtr++;
//			if (*keyPtr == '\0')//�� ã�����
//			{
//
//				if (sm == FULL_MATCH)
//				{
//					if (pTN->getKey() == '\0')//Ű���尡 ������ ���
//					{
//						return pTN;
//					}
//					else //Ű�� �� ����(Ǯ ��ġ�� ������)
//					{
//						return NULL;//ã�� ����
//					}
//				}
//				else if (sm == PREFIX_MATCH)//���ξ� ��ġ
//				{
//					return pTN;
//				}
//			}
//			else if ((pTN->getKey() == '\0') && (*keyPtr != '\0'))//Ű�� �������
//			{
//				if (pTN->getNext() != NULL)
//				{
//					pTN = pTN->getNext();//������ġ���� ã�� �� �ִٸ� ã��
//					continue;
//				}
//				else
//					return NULL;//ã�� �� ����
//			}
//			else
//			{
//				continue;
//			}
//		}
//	}
//}
//
//template<typename E>
//void Trie<E>::_traverse(TrieNode<E>* pTN, List_pVoca& list_keywords)
//{
//	if (pTN == NULL)
//		return;
//	if (pTN->getChild() == NULL)//������ ����ϰ��
//	{
//		list_keywords.push_back(pTN->getValue());//value �޾ƿ�
//	}
//	else
//	{
//		_traverse(pTN->getChild(), list_keywords);//�Ʒ��� ��ӳ�����
//	}
//	if (pTN->getNext() != NULL)//������ �����Ѵٸ�
//	{
//		_traverse(pTN->getNext(), list_keywords);//��� �������� �̵�
//	}
//}
//
//template<typename E>
//void Trie<E>::findPrefixMatch(string prefix, List_pVoca& predictWords)
//{
//	TrieNode<E>* pPtr = NULL;
//	const char* keyPtr = (char*)prefix.c_str();
//	TrieNode<E>* pTN = NULL;
//	TrieNode<E>* found = NULL;
//	keyPtr;
//	if (keyPtr == NULL)//���ڰ� �������
//		return;
//	pTN = this->_root;//��Ʈ���� ����
//	pTN = _find(prefix, PREFIX_MATCH);//���ξ� ��ġ�� ��ġ ã��
//
//	_traverse(pTN, predictWords);//�ܾ�� ����Ʈ�� ����
//
//}
//
//template<typename E>
//void Trie<E>::deleteKeyStr(string KeyStr)
//{
//
//	TrieNode<E>* pTN = NULL, * _root;
//	TrieNode<E>* tmp = NULL;
//	int trie_val;
//
//	_root = this->_root;//��Ʈ��� ����
//	if (NULL == _root || "" == KeyStr)//����� ����
//		return;
//
//	pTN = _find(KeyStr, FULL_MATCH);//��ġ ã��
//
//	if (pTN == NULL)
//	{
//		cout << "Key [" << KeyStr << "] not found in trie" << endl;
//		return;
//	}
//
//	while (true)
//	{
//		if (pTN == NULL)
//			break;
//		if (pTN->getPrev() && pTN->getNext())//������� ��� ������ ������ ���� �߰�������
//		{
//			tmp = pTN;
//			//������ ������ ��������
//			(pTN->getNext())->setPrev(pTN->getPrev());
//			(pTN->getPrev())->setNext(pTN->getNext());
//			free(tmp);//��ȯ
//			break;
//		}
//		else if (pTN->getPrev() && !(pTN->getNext()))//������ ������� ��尡 ����
//		{
//			tmp = pTN;
//			(pTN->getPrev())->setNext(NULL);//��������� ������ NULL�μ���
//			free(tmp);//��ȯ
//			break;
//		}
//		else if (!(pTN->getPrev()) && pTN->getNext())//������ ��������尡 ����
//		{
//			tmp = pTN;
//			(pTN->getParent())->setChild(pTN->getNext());//�θ��ڽİ��踦 �ٲ��� ������尡 �ڽ��̵Ǵ� ��
//			pTN = pTN->getNext();//������ġ�� �Ű���
//			pTN->setPrev(NULL);//���ο� ��ġ�� ���� �� ������������� NULL�μ���
//			free(tmp);//��ȯ
//			break;
//		}
//		else
//		{
//			tmp = pTN;
//			pTN = pTN->getParent();//�θ���� �̵�
//			if (pTN != NULL)
//				pTN->setChild(NULL);//�ڽ��� NULL�� ����
//			free(tmp);//��ȯ
//			if ((pTN == _root) && (pTN->getNext() == NULL) && (pTN->getPrev() == NULL))//��Ʈ��常 ����
//			{
//				cout << "Now, the trie is empty !!" << endl;
//				break;
//			}
//		}
//	}
//	this->num_keys--;//Ű�� ���� �ٿ���
//}
//
//template<typename E>
//void Trie<E>::eraseTrie()
//{
//	TrieNode<E>* pTN;
//	TrieNode<E>* pTN_to_be_deleted = NULL;
//	if (this->_root == NULL)//������� ����
//		return;
//	pTN = this->_root;
//	/* delete the last key word first */
//	while (pTN != NULL)//��������� ������ �ݺ�
//	{
//		while ((pTN != NULL) && (pTN->getNext()))//������尡 �������
//			pTN = pTN->getNext();//������ġ�� �������̵�
//		while (pTN->getChild())//�ڽ��� �����
//		{
//			if (pTN->getNext())
//				break;
//			pTN = pTN->getChild();
//		}
//		if (pTN->getNext())
//			continue;
//		if (pTN->getPrev() && pTN->getNext())
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getNext())->setPrev(pTN->getPrev());
//			(pTN->getPrev())->setNext(pTN->getNext());
//			pTN = pTN->getNext();
//			free(pTN_to_be_deleted);
//		}
//		else if (pTN->getPrev() && !(pTN->getNext()))
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getPrev())->setNext(NULL);
//			pTN = pTN->getPrev();
//			free(pTN_to_be_deleted);
//		}
//		else if (!(pTN->getPrev()) && pTN->getNext())
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getParent())->setChild(pTN->getNext());
//			(pTN->getNext())->setPrev(NULL);
//			pTN = pTN->getNext();
//			free(pTN_to_be_deleted);
//		}
//		else
//		{
//			pTN_to_be_deleted = pTN;
//			if (pTN == this->_root)
//			{
//				/* _root */
//				this->num_keys = 0;
//				return;
//			}
//			if (pTN->getParent() != NULL)
//			{
//				pTN = pTN->getParent();
//				pTN->setChild(NULL);
//			}
//			else
//			{
//				pTN = pTN->getPrev();
//			}
//			free(pTN_to_be_deleted);
//		} // end if - else
//	} // end while
//}
//
//template<typename E>
//void Trie<E>::fprintTrie(ostream& fout)
//{
//	TrieNode<E>* pTN;
//	int line = 1, indent = 0;
//	fout << "trie ( " << this->trie_name << ") with " << this->num_keys << " trie_nodes\n";
//	if (this->num_keys == 0)
//	{
//		fout << "Empty trie !" << endl;
//		return;
//	}
//	pTN = this->_root;
//	pTN->_fprint(fout, pTN, indent);
//}
//#endif

// =============================================================================================================
// �տ��� ���� ��

//#ifndef Trie_H
//#define Trie_H
//#include "TrieNode.h"
//#define MAX_STR_LEN 50
//#define ISITRIGHT
//using namespace std;
//
//typedef list<MyVoca*> List_pVoca; //���̺�ī��_����Ʈ
//typedef list<MyVoca*>::iterator List_pVoca_Iter; //����Ʈ �ݺ���
//
//enum SearchMode { FULL_MATCH, PREFIX_MATCH };
//
//template <typename E>
//class Trie
//{
//public:
//	Trie(string name);
//	int size() { return num_keys; }
//	void insert(string keyStr, E value);
//	void insertExternalTN(TrieNode<E>* pTN, string keyStr, E value); //no child
//	TrieNode<E>* find(string keyStr);
//	void findPrefixMatch(string prefix, List_pVoca& predictWords);
//	void deleteKeyStr(string keyStr); //�� Ű���� ����
//	void eraseTrie(); //Ʈ���� ��ü ����
//	void fprintTrie(ostream& fout);
//
//protected:
//	TrieNode<E>* _find(string keyStr, SearchMode sm = FULL_MATCH);
//	void _traverse(TrieNode<E>* pTN, List_pVoca& list_keywords);
//
//private:
//	TrieNode<E>* _root;
//	int num_keys; //�� Ű����(�ܾ�)�� ���Ѱ���
//	string trie_name;
//};
//
///*������*/
///*������*/
//template<typename E>
//Trie<E>::Trie(string name)
//{
//	trie_name = name;
//
//	/*��Ʈ ����*/
//	_root = new TrieNode<E>('\0', NULL);
//	_root->setKey('\0');
//	_root->setPrev(NULL);
//	_root->setNext(NULL);
//	_root->setParent(NULL);
//	_root->setChild(NULL);
//	num_keys = 0;
//}
//
///*�Ѵܾ� ���� �˸��� Trie Node ����*/
///*pTN ������ �Ѵܾ��, ��� �� �ؿ� �پ� �Ѵܾ� ���� �˸�
//->Ű�� '\0', VALUE�� ����*/
///*�� �μ�Ʈ ���� �������� ����*/
//template<typename E>
//void Trie<E>::insertExternalTN(TrieNode<E>* pTN, string keyStr, E value)
//{
//	TrieNode<E>* pTN_New = NULL;
//	pTN_New = new TrieNode<E>('\0', value); //��� ����
//
//	pTN->setChild(pTN_New);
//	(pTN->getChild())->setParent(pTN);
//	pTN_New->setValue(value); //�� �������� ���ʿ� �ߴµ� �̰� ���ϳ�
//}
//
///*insert(keyword, value) to Trie*/
//template<typename E>
//void Trie<E>::insert(string keyStr, E value)
//{
//	TrieNode<E>* pTN = NULL, * pTN_New = NULL;
//
//	/*string ���� �Լ���, ��ȯ���� char**/
//	char* keyPtr = (char*)keyStr.c_str(); //��Ʈ���� C��Ÿ���� ���ڿ��� �ٲٴ� �Լ���
//
//	/*Ű����(�ܾ�)�� ������ ����*/ /*keyPtr�̶� ���ڿ���*/ /* *keyPtr�� ������ */
//	if (keyPtr == NULL)
//		return;
//
//	/*Ǯ��ġ ���� Ű���� Ž��*/ /*������ �� ��ȯ*/
//	/*�̹� Ʈ���̿� �ִ� Ű����(���ڿ�) �ΰ���*/
//	if (_find(keyStr, FULL_MATCH) != NULL)
//	{
//		cout << "The given key string (" << keyStr << ") is already existing; just return." << endl;
//		return;
//	}
//
//	/*��Ʈ����*/
//	pTN = this->_root;
//
//	/*���� ��ġ ã�°���*/
//	/*pTN�� �����ϰ�, ������ Ű����(���ڿ�)�� ������ ���� ����*/
//	while ((pTN != NULL) && (*keyPtr != '\0'))
//	{
//		/*pTN�� Ű(����)���� ������ Ű(����)�� �� ũ��, */
//		/*pTN�� �������� ���ٸ� while ����. �ŵ� ������ ��*/
//		if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))
//			break;
//
//		/*pTN�� Ű(����)���� ������ Ű(����)�� �� ũ��,*/
//		/*�������� �����Ѵٸ� ���������� �Ѿ ���*/
//		while ((pTN->getKey() < *keyPtr) && (pTN->getNext() != NULL))
//			pTN = pTN->getNext();
//
//		/*Ʈ���̿� �ִ� Ű(����)��. �� �ܾ� Ÿ�� ������*/
//		/*������ Ű����(�ܾ�)�� ������ ��������*/
//		/*�׷� ���� ������ ����.*/
//		/*1)���� �ܾ�� ���� 2)�����ܾ�� ū 3) �����ܾ� ������ �Ȱ��� �߰��� ����*/
//		while ((pTN != NULL) && (pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))
//		{
//			pTN = pTN->getChild();
//			keyPtr++;
//		}
//
//		/*���� ������ Ű����(�ܾ�)�� �ȳ�����, pTN��ġ�� Ű(����)�� �� ũ�ٸ� Ż��*/
//		/*������� stance�� �־��µ�, stack�� ����*/
//		if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))
//			break;
//	} //���� ������ ��ġ �������� //end of while
//
//
//	/*�ϳ�*/
//	/*�� ���� 1)�����. there was a longer key string already*/
//	/*action �ִµ� act �ִ°���*/
//	if ((pTN->getKey() != '\0') && (*keyPtr == '\0'))
//	{
//		/* break the longer key string, and connected to the separated key strings */
//		pTN_New = new TrieNode<E>('\0', value); //external node
//		pTN_New->setParent(pTN->getParent());
//		(pTN->getParent())->setChild(pTN_New); //action�� i�� *pTN��
//		pTN_New->setNext(pTN);
//		pTN->setPrev(pTN_New);
//
//		//cout << "key (" << keyWord << ") is inserted" << endl;
//		this->num_keys++; //�� Ű����(�ܾ�)�� ���س�
//		return;
//	}
//
//	/*��*/
//	/*aged �־���, agenda �ִ� ���̽�. pTN: d*/
//	else if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))
//	{
//		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE); //'n'��
//		pTN_New->setParent(pTN->getParent());
//		pTN_New->setPrev(pTN);
//		pTN->setNext(pTN_New);
//
//		pTN = pTN_New; //'n'
//		keyPtr++;
//		while (*keyPtr != '\0') //������ Ű����(�ܾ�)�� ������
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE); //�� ��� ����
//			pTN->setChild(pTN_New);
//			(pTN->getChild())->setParent(pTN); //�� pTN_New�� �θ����̶� �ϸ� �ɰ� ����
//
//			pTN = pTN->getChild(); //�Űܰ��� �ٽ� ����
//			keyPtr++;
//		}
//
//		/*�����Ұ� ����. pTN:agenda�� ������ 'a'*/
//		if (*keyPtr == '\0')
//		{
//			insertExternalTN(pTN, keyStr, value);
//			this->num_keys++;
//			return;
//		}
//	}
//
//	/*��*/
//	/*stance �־��µ� stack �ִ°���. pTN:stance�� 'n'*/
//	else if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))
//	{
//		/* insert between two existing trie nodes */
//		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE); //'c'
//		pTN_New->setNext(pTN);
//		pTN_New->setParent(pTN->getParent());
//
//		if (pTN->getPrev() == NULL)
//		{
//			if (pTN->getParent() != NULL)
//				(pTN->getParent())->setChild(pTN_New);
//		}
//
//		/*�̰� �� star�� stack�־��µ� stance �ִ°�. pTN:star�� 'r'*/
//		else  //pTN_New :stance�� 'n'
//		{
//			(pTN->getPrev())->setNext(pTN_New);
//		}
//
//		pTN_New->setPrev(pTN->getPrev());
//		pTN->setPrev(pTN_New);
//		pTN = pTN_New;
//		keyPtr++;
//
//		/*������ Ű����(�ܾ�) ������ ����*/
//		while (*keyPtr != '\0')
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);
//			pTN->setChild(pTN_New);
//			(pTN->getChild())->setParent(pTN);
//			pTN = pTN->getChild();
//			keyPtr++;
//		}
//
//		/*������ Ű����(�ܾ�)�� ��*/
//		if (*keyPtr == '\0')
//		{
//			insertExternalTN(pTN, keyStr, value);
//			this->num_keys++;
//			return;
//		}
//	}
//}
//
///*find(keyWord) -> ��� ������ ��ȯ*/
//template<typename E>
//TrieNode<E>* Trie<E>::find(string keyWord)
//{
//	TrieNode<E>* pTN = NULL;
//	pTN = _find(keyWord, FULL_MATCH);
//	return pTN;
//}
//
///*_find(keyWord(�ܾ�), SearchMode) -> ��� ������ ��ȯ*/
////Ǯ�̸� �ش� �ܾ��� ��, ������ ��ġŰ������ ���� ����
//template<typename E>
//TrieNode<E>* Trie<E>::_find(string keyStr, SearchMode sm)
//{
//	/*string�� C��Ÿ�� ���ڿ��� �ٲٴ� �Լ�*/
//	const char* keyPtr = keyStr.c_str();
//	TrieNode<E>* pTN = NULL;
//	TrieNode<E>* found = NULL;
//
//	/*Ű���尡 ����*/ /*keyPtr�̶� ���ڿ���*/
//	if (keyPtr == NULL)
//		return NULL;
//
//	/*��Ʈ���� �˻�*/
//	pTN = this->_root;
//
//	/*���̻� ��尡 ���ų�, ��ġŰ���尡 ���� '\0'�϶� Ż��*/
//	/* *keyPtr�̶� ���ڸ� ��Ÿ��*/
//	while ((pTN != NULL) && (*keyPtr != '\0'))
//	{
//		/*pTN�� �ְ�, pTN�� Ű(����)�� ã�� Ű(����)���� �������� ����������*/
//		/*�̷��� ��ġŰ(����)�� ���ų�,*/
//		/*��ġŰ(����)�� ������ �� ū ���(����)�� �Űܰ� pTN��*/
//		while ((pTN != NULL) && (pTN->getKey() < *keyPtr))
//		{
//			if (pTN->getNext() == NULL)
//				return NULL;
//			pTN = pTN->getNext();
//		}
//
//		/*pTN�� �ִµ�, �װ� ��ġŰ(����)�� �ٸ���*/
//		if ((pTN != NULL) && (pTN->getKey() > *keyPtr))
//		{
//			return NULL; //��ã������
//		}
//
//		/*���������� �Űܿ� pTN�� ���°���, ��ġŰ���嵵 �ȳ�����*/
//		else if ((pTN == NULL) && (*keyPtr != '\0'))
//		{
//			return NULL; //��ã������
//		}
//
//		/*pTN�� Ű(����)�� ��ġŰ(����)�� ������, ��ġŰ����(�ܾ�)�� �ȳ�����*/
//		else if ((pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))
//		{
//			pTN = pTN->getChild(); //�ڽ����� ����
//			keyPtr++; //����Ű(����)�� ��
//
//			/*��ġŰ����(�ܾ�) ������ �Ҹ���*/
//			if (*keyPtr == '\0')
//			{
//				/* key or prefix found*/
//				if (sm == FULL_MATCH)
//				{
//					/*pTN�ڽ����� �Դ��� �Ѵܾ��� Ǯ��ġ�� �� ã������*/
//					if (pTN->getKey() == '\0')
//					{
//						return pTN;
//					}
//					/*pTN�� �ڽ����� �Դµ�, Ʈ���� �� �ܾ�� �ȳ�����. Ǯ��ġ �ƴ�, ��, ��ã���� ó����*/
//					else //(pTN->getKey() != '\0')
//					{
//						return NULL;
//					}
//				}
//
//				/*Ǯ��ġ �ƴ� �� ã���� �Ȱ���*/
//				/*�� ��ġŰ���尡 �� ���Ե������� �װɷ� �Ȱ���*/
//				else if (sm == PREFIX_MATCH)
//				{
//					return pTN;
//				}
//			}
//
//			/*�ڽ����� �Դ���*/
//			/*Ʈ���� �� �Ѵܾ�� ���ε� ��ġŰ����(�ܾ�)�� �ȳ����� �Ҹ���*/
//			/*������� action ã����, act�� '\0'���� �°���*/
//			else if ((pTN->getKey() == '\0') && (*keyPtr != '\0'))
//			{
//				if (pTN->getNext() != NULL) //������ �Űܰ��� �ٽ� ����
//				{
//					pTN = pTN->getNext();
//					continue;
//				}
//				else
//					return NULL;
//			}
//
//			/*Ʈ���� �� �Ѵܾ� ���� �ƴϾ����� �ٽ� ������*/
//			else
//				continue;
//		}
//	}
//}
//
///*��ȸ-> ���̺�ī.������.��_����Ʈ�� ����*/
//template<typename E>
//void Trie<E>::_traverse(TrieNode<E>* pTN, List_pVoca& list_keywords)
//{
//	if (pTN == NULL)
//		return;
//
//	/* pTN: external node*/
//	if (pTN->getChild() == NULL)
//	{
//		list_keywords.push_back(pTN->getValue()); //value: myvoca*
//	}
//	/* pTN: internal node*/
//	else
//	{
//		_traverse(pTN->getChild(), list_keywords);
//	}
//
//	/*�ش� �����Ƚ� ���� ���� �ܾ����*/
//	if (pTN->getNext() != NULL)
//	{
//		_traverse(pTN->getNext(), list_keywords);
//	}
//}
//
///*���η� Ž��*/
//template<typename E>
//void Trie<E>::findPrefixMatch(string keyStr, List_pVoca& predictWords)
//{
//	TrieNode<E>* pPtr = NULL;
//	const char* keyPtr;
//	TrieNode<E>* pTN = NULL;
//	TrieNode<E>* found = NULL;
//	keyPtr = keyStr.c_str();
//
//	if (keyPtr == NULL)
//		return;
//
//	pTN = this->_root; //Ʈ���� ��Ʈ����
//	pTN = _find(keyStr, PREFIX_MATCH);
//
//	/*�� �ֿ����!!*/
//	_traverse(pTN, predictWords);
//}
//
//
///*Ű����(�ܾ�) ����*/
//template<typename E>
//void Trie<E>::deleteKeyStr(string keyWord)
//{
//	TrieNode<E>* pTN = NULL, * _root;
//	TrieNode<E>* tmp = NULL;
//	int trie_val;
//
//	_root = this->_root;
//	if (_root == NULL || keyWord.c_str() == NULL)
//		return;
//
//	pTN = _find(keyWord, FULL_MATCH); //Ǯ�̸� �ش� �ܾ��� ��(external node), ������ ��ġŰ������ ���� ����
//	if (pTN == NULL)
//	{
//		cout << "Key [" << keyWord << "] not found in trie" << endl;
//		return;
//	}
//
//	while (1)
//	{
//		/*������ Ű����(�ܾ�) �� ������*/
//		if (pTN == NULL)
//			break;
//#ifndef ISITRIGHT
//		/*�ְ�, �ְ�. ���� star ���� �Ѵٺ���. star�� extern �տ��� �� �ִ� �����ϼ�. pTN: extern nod*/
//		if (pTN->getPrev() && pTN->getNext())
//		{
//			tmp = pTN;
//			(pTN->getNext())->setPrev(pTN->getPrev());
//			(pTN->getPrev())->setNext(pTN->getNext());
//
//			free(tmp); //��� ����
//			break; //break??? what about r?? ��! stare�� ���� �ֳ�
//		}
//
//		/*�ְ�, ����. ���� stare ����, extern ���ʿ� ���ִ� �����ϼ�.*/
//		/*stare ���� �ȵǰڴ�. stare�� ���� �����ϰ�, star �����Ѵ� ����. extern ���ʿ� �� �ְ�*/
//		/*�� �̰͵� �ȵǳ� ���ʿ� extern ���ʿ� �����Ҽ��� �ֳ�?*/
//		/*�ƴ� ���ļ��̶� ���ʿ� extern ������ �����Ҽ��� ���� �ʳ�?*/
//		else if (pTN->getPrev() && !(pTN->getNext()))
//		{
//			tmp = pTN;
//			(pTN->getPrev())->setNext(NULL);
//
//			free(tmp); //extern �� ���� �Ǵ°���. 
//			break;
//		}
//#endif		
//		int p = 0; //�̰� ���� if ��������ø� ���� �ִ� ������
//		if (p == 1);
//
//		/*����. ���� act ����. ��ư ������ �ִܰ� �̾����°� �ִܰŴϱ� ���� �� �����ʿ� ����*/
//		else if (!(pTN->getPrev()) && pTN->getNext())
//		{
//			tmp = pTN;
//
//			(pTN->getParent())->setChild(pTN->getNext());
//			pTN = pTN->getNext();
//			pTN->setPrev(NULL);
//
//			free(tmp); //extern nod����
//			break;
//		}
//
//		/*����. ������ �� ������� ��. act�� cat ���־���, act ����� �����ϼ�.*/
//		else
//		{ //pTN�� external node
//			tmp = pTN;
//			pTN = pTN->getParent(); //����. while������ ������ ��� ������
//
//			if (pTN != NULL) //�Ʒ����� ���� ���������Ŵϱ�
//				pTN->setChild(NULL);
//
//			free(tmp);
//
//			/*��Ʈ���� �°���*/ /*Ʈ���� �� ù ����� prev��*/
//			if ((pTN == _root) && (pTN->getNext() == NULL) && (pTN->getPrev() == NULL))
//			{
//				cout << "Now, the trie is empty !!" << endl;
//				break;
//			}
//		}
//	}
//	this->num_keys--;
//}
//
///*Ʈ���� ��ü ����*/
//template<typename E>
//void Trie<E>::eraseTrie()
//{
//	TrieNode<E>* pTN;
//	TrieNode<E>* pTN_to_be_deleted = NULL;
//
//	if (this->_root == NULL) //��Ʈ�� ���°�
//		return;
//
//	pTN = this->_root; //��Ʈ���� Ž��
//
//	/* delete the last key word first */
//	while (pTN != NULL) //�� �� �����Ǳ� ������ ��� ����
//	{
//		while ((pTN != NULL) && (pTN->getNext())) //��� ����������
//			pTN = pTN->getNext();
//
//		while (pTN->getChild()) //��� �Ʒ���
//		{
//			if (pTN->getNext()) //���� �����ʿ� ���� ��� ���߰�
//				break;
//			pTN = pTN->getChild();
//		}
//
//		if (pTN->getNext()) //���������� ������ �ٽ� �� while������ 
//			continue;
//		/*�׷� ���� ���ϴ� �ֿ��� ������*/
//
//
//		/*�� ������ ����� ������ delete ���� �ȵǳ� ��*/
//		/*�� ���� ��ȯ�� ��� ���谡 �ȵǰڴ�. ��Ƶΰ� �ϸ� ����?*/
//		/*�ٵ� �̰� next�� ������ �ֳ�? ����. ������ �ִ� �� ������ ������ ��*/
//		/*����*/
//		if (pTN->getPrev() && pTN->getNext())
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getNext())->setPrev(pTN->getPrev());
//			(pTN->getPrev())->setNext(pTN->getNext());
//			pTN = pTN->getNext();
//			free(pTN_to_be_deleted);
//		}
//
//		/*�־�*/
//		else if (pTN->getPrev() && !(pTN->getNext()))
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getPrev())->setNext(NULL);
//			pTN = pTN->getPrev();
//			free(pTN_to_be_deleted);
//		}
//
//		/*����*/
//		/*else if (!(pTN->getPrev()) && pTN->getNext())
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getParent())->setChild(pTN->getNext());
//			(pTN->getNext())->setPrev(NULL);
//			pTN = pTN->getNext();
//			free(pTN_to_be_deleted);
//		}*/
//
//		/*����*/
//		else
//		{
//			pTN_to_be_deleted = pTN;
//			if (pTN == this->_root)
//			{
//				this->num_keys = 0; //���ص� �ɰ� ������ �������ΰ� ������ �����ϸ� �����ݾ�..? ���ϳ� ������ �ƹ���
//				return;
//			}
//			if (pTN->getParent() != NULL)
//			{
//				pTN = pTN->getParent();
//				pTN->setChild(NULL); //�Ʒ��� ���� �����ϴϱ�
//			}
//			else //���� �θ����
//			{
//				pTN = pTN->getPrev();
//			}
//
//			free(pTN_to_be_deleted);
//		}
//	} //end while
//}
//
///*Ʈ���� ���*/
//template<typename E>
//void Trie<E>::fprintTrie(ostream& out)
//{
//	TrieNode<E>* pTN;
//	int line = 1, indent = 0;
//
//	out << "trie ( " << this->trie_name << ") with "
//		<< this->num_keys << " trie_nodes\n";
//
//	if (this->num_keys == 0)
//	{
//		out << "Empty trie !" << endl;
//		return;
//	}
//
//	pTN = this->_root;
//	pTN->_fprint(out, pTN, indent);
//}
//#endif