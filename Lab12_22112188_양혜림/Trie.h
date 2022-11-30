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

// =============================================================================================================
// 차태현 선배 거

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
//	Trie(string name);//생성자
//	int size() { return num_keys; }//크기 반환
//	void insert(string keyStr, E value);//삽입
//	void insertExternalTN(TrieNode<E>* pTN, string keyStr, E value);//External노드 삽입
//	TrieNode<E>* find(string keyStr);//키워드 탐색
//	void findPrefixMatch(string prefix, List_pVoca& predictVocas);//동일한 접두어 가진 단어출력
//	void deleteKeyStr(string keyStr);//키워드를 가진노드 삭제
//	void eraseTrie();//전체 삭제
//	void fprintTrie(ostream& fout);//출력
//protected:
//	TrieNode<E>* _find(string keyStr, SearchMode sm = FULL_MATCH);//키워드위치에 노드 반환 서치모드에 따라서 
//	void _traverse(TrieNode<E>* pTN, List_pVoca& list_keywords);//키워드 리스트에 담아줌
//private:
//	TrieNode<E>* _root;//루트노드
//	int num_keys;//키의 수
//	string trie_name;//이름
//};
//
//template<typename E>
//Trie<E>::Trie(string name)
//{
//	trie_name = name;//이름설정
//	_root = new TrieNode<E>(' ', NULL);//루트노드 생성
//	_root->setKey(' ');//키 설정
//	_root->setPrev(NULL);//이전 초기화
//	_root->setNext(NULL);//다음 초기화
//	_root->setParent(NULL);//부모 초기화
//	_root->setChild(NULL);//자식 초기화
//	num_keys = 0;
//}
//
//
//template<typename E>
//void Trie<E>::insertExternalTN(TrieNode<E>* pTN, string keyStr, E value)
//{
//	TrieNode<E>* pTN_New = NULL;
//
//	pTN_New = new TrieNode<E>('\0', value);//external노드 생성
//	pTN->setChild(pTN_New);//pTN의 자식으로 설정
//	(pTN->getChild())->setParent(pTN);//pTN_New 부모는 pTN
//	pTN_New->setValue(value);//값 정의
//}
//
//template<typename E>
//void Trie<E>::insert(string keyStr, E value)
//{
//	TrieNode<E>* pTN = NULL, * pTN_New = NULL;
//	char* keyPtr = (char*)keyStr.c_str();//단어를 스펠링 하나하나로 관리
//
//	if (keyStr == "")
//		return;
//	if (_find(keyStr, FULL_MATCH) != NULL)
//	{
//		cout << "The given key string (" << keyStr << ") is already existing; just return !!" << endl;//키워드가 이미 존재할 경우
//		return;
//	}
//
//	pTN = this->_root;//루트노드로 설정
//
//	while ((pTN != NULL) && (*keyPtr != '\0'))//pTN이 NULL이 아니고 삽입하려는 키가 존재할 경우
//	{
//		if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))//삽입위치의 키값이 기존 키값보다 크고 바로 다음위치가 NULL일경우 
//			break;//바로 다음에 삽입하면 되기때문에 브레이크
//		while ((pTN->getKey() < *keyPtr) && (pTN->getNext() != NULL))//pTN 다음 위치에 노드가 이미 존재할 경우
//		{
//			pTN = pTN->getNext();//다음으로 넘어가서 다음이 NULL이 될때까지 실행
//		}
//		while ((pTN != NULL) && (pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))//키가 동일한 경우
//		{
//			pTN = pTN->getChild();//자식노드로 이동
//			keyPtr++;
//		}
//		if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))//pTN앞에 위치에 삽입하는 경우
//			break;
//	}
//
//	//삽입할려는 키워드가 기존키워드에 포함되어있는경우
//	if ((pTN->getKey() != '\0') && (*keyPtr == '\0'))//현재 노드의 키는 더 삽입키워드를 포함하면서 스펠링이 남아있고 삽입할려는 단어의 스펠링의 마지막인경우
//	{
//		//내가 삽입할려는문자를 포함하는 더 긴문자 존재
//		pTN_New = new TrieNode<E>('\0', value);//노드 생성
//		pTN_New->setParent(pTN->getParent());//기존노드의 부모를 부모노드로 설정
//		(pTN->getParent())->setChild(pTN_New);//기존 부모노드의 자식노드를 새롭게 생성한 노드로설정
//		pTN_New->setNext(pTN);//삽입할려는 키의 마지막노드의 다음으로 문자의 나머지값들을 설정
//		pTN->setPrev(pTN_New);//이전노드로 설정해서 연결
//		this->num_keys++;//키를 증가
//
//		return;
//	}
//	else if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))//바로 오른쪽에 삽입가능하여 단어가 존재
//	{
//
//		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//키워드를 갖는 노드생성
//		pTN_New->setParent(pTN->getParent());//부모노드는 같음
//		pTN_New->setPrev(pTN);//이전을 pTN
//		pTN->setNext(pTN_New);//이후를 pTN_New 서로 연결
//		pTN = pTN_New;//다음으로 이동
//		keyPtr++;
//		while (*keyPtr != '\0')//문자가 존재할경우
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//노드 생성
//			pTN->setChild(pTN_New);//자식으로 설정
//			(pTN->getChild())->setParent(pTN);//부모로 설정 서로 연결
//			pTN = pTN->getChild();//아래위치로 내려감
//			keyPtr++;
//		}
//		if (*keyPtr == '\0')//문자가 끝났음
//		{
//			insertExternalTN(pTN, keyStr, value);//마지막 노드 추가
//			this->num_keys++;
//			return;
//		}
//	}
//
//	else if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))//pTN위치보다 앞에 삽입되는경우
//	{
//
//		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//노드 생성
//		pTN_New->setNext(pTN);//pTN보다 이전으로 설정
//		pTN_New->setParent(pTN->getParent());//부모노드는 같음
//		if (pTN->getPrev() == NULL)//이전이 NULL이었다면 레벨에서 첫번째 노드임
//		{
//			if (pTN->getParent() != NULL)//부모가 있다면
//				(pTN->getParent())->setChild(pTN_New);//부모 자식 연결
//		}
//		else
//		{
//			(pTN->getPrev())->setNext(pTN_New);//pTN의 이전노드의 다음으로 설정
//		}
//		pTN_New->setPrev(pTN->getPrev());//새로생성된 노드의 이전은 기존노드의 이전
//		pTN->setPrev(pTN_New);//pTN이전은 새로 생성된 노드
//		pTN = pTN_New;//이전으로 이동
//		keyPtr++;//다음 글자처리를 위함
//		while (*keyPtr != '\0')//문자가 있을경우
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);//노드 생성
//			pTN->setChild(pTN_New);//자식 설정
//			(pTN->getChild())->setParent(pTN);//부모자식 연결
//			pTN = pTN->getChild();//아래로 이동
//			keyPtr++;//다음 문자처리를 위해
//		}
//		if (*keyPtr == '\0')//문자를 다처리했을경우
//		{
//			insertExternalTN(pTN, keyStr, value);//External노드 추가
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
//	TrieNode<E>* pTN = NULL;//노드 생성
//	pTN = _find(keyStr, FULL_MATCH);//키워드와 완전히 일치하는 노드
//	return pTN;
//}
//
//template<typename E>
//TrieNode<E>* Trie<E>::_find(string keyStr, SearchMode sm)
//{
//	const char* keyPtr = (char*)keyStr.c_str();
//	TrieNode<E>* pTN = NULL;//노드 생성
//	TrieNode<E>* found = NULL;//찾은 노드
//	if (keyPtr == NULL)//찾을키가 없으면
//		return NULL;
//
//	pTN = this->_root;//루트노드로 설정
//	while ((pTN != NULL) && (*keyPtr != '\0'))//노드가 존재하고 문자가 존재하면
//	{
//		while ((pTN != NULL) && (pTN->getKey() < *keyPtr))//찾을려는 문자의 키값이 더큰경우
//		{
//			if (pTN->getNext() == NULL)//다음이 NULL이면
//				return NULL;//문자가 존재하지 않음
//			pTN = pTN->getNext();//다음노드로 이동
//		}
//		if ((pTN != NULL) && (pTN->getKey() > *keyPtr))//찾을려는 문자의 키가 더큰경우(이미 다음으로 이동했기에)
//		{
//			return NULL;//찾지못함
//		}
//		else if ((pTN == NULL) && (*keyPtr != '\0'))//문자는 존재하나 이동할 노드가없음
//		{
//			return NULL;//찾지못함
//		}
//		else if ((pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))//문자가 동일하며 문자가 남아있는경우
//		{
//			pTN = pTN->getChild();//아래로 설정
//			keyPtr++;
//			if (*keyPtr == '\0')//다 찾은경우
//			{
//
//				if (sm == FULL_MATCH)
//				{
//					if (pTN->getKey() == '\0')//키워드가 동일한 경우
//					{
//						return pTN;
//					}
//					else //키가 더 긴경우(풀 매치기 때문에)
//					{
//						return NULL;//찾지 못함
//					}
//				}
//				else if (sm == PREFIX_MATCH)//접두어 일치
//				{
//					return pTN;
//				}
//			}
//			else if ((pTN->getKey() == '\0') && (*keyPtr != '\0'))//키가 남은경우
//			{
//				if (pTN->getNext() != NULL)
//				{
//					pTN = pTN->getNext();//다음위치에서 찾을 수 있다면 찾음
//					continue;
//				}
//				else
//					return NULL;//찾을 수 없음
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
//	if (pTN->getChild() == NULL)//마지막 노드일경우
//	{
//		list_keywords.push_back(pTN->getValue());//value 받아옴
//	}
//	else
//	{
//		_traverse(pTN->getChild(), list_keywords);//아래로 계속내려감
//	}
//	if (pTN->getNext() != NULL)//다음이 존재한다면
//	{
//		_traverse(pTN->getNext(), list_keywords);//계속 다음으로 이동
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
//	if (keyPtr == NULL)//문자가 없을경우
//		return;
//	pTN = this->_root;//루트노드로 설정
//	pTN = _find(prefix, PREFIX_MATCH);//접두어 일치로 위치 찾음
//
//	_traverse(pTN, predictWords);//단어들 리스트에 저장
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
//	_root = this->_root;//루트노드 설정
//	if (NULL == _root || "" == KeyStr)//대상이 없음
//		return;
//
//	pTN = _find(KeyStr, FULL_MATCH);//위치 찾기
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
//		if (pTN->getPrev() && pTN->getNext())//삭제대상 노드 이전과 다음이 존재 중간노드삭제
//		{
//			tmp = pTN;
//			//이전과 다음을 직접연결
//			(pTN->getNext())->setPrev(pTN->getPrev());
//			(pTN->getPrev())->setNext(pTN->getNext());
//			free(tmp);//반환
//			break;
//		}
//		else if (pTN->getPrev() && !(pTN->getNext()))//이전에 삭제대상 노드가 존재
//		{
//			tmp = pTN;
//			(pTN->getPrev())->setNext(NULL);//이전노드의 다음을 NULL로설정
//			free(tmp);//반환
//			break;
//		}
//		else if (!(pTN->getPrev()) && pTN->getNext())//다음에 삭제대상노드가 존재
//		{
//			tmp = pTN;
//			(pTN->getParent())->setChild(pTN->getNext());//부모자식관계를 바꿔줌 다음노드가 자식이되는 것
//			pTN = pTN->getNext();//다음위치로 옮겨줌
//			pTN->setPrev(NULL);//새로운 위치의 이전 즉 기존삭제대상노드 NULL로설정
//			free(tmp);//반환
//			break;
//		}
//		else
//		{
//			tmp = pTN;
//			pTN = pTN->getParent();//부모노드로 이동
//			if (pTN != NULL)
//				pTN->setChild(NULL);//자식을 NULL로 설정
//			free(tmp);//반환
//			if ((pTN == _root) && (pTN->getNext() == NULL) && (pTN->getPrev() == NULL))//루트노드만 존재
//			{
//				cout << "Now, the trie is empty !!" << endl;
//				break;
//			}
//		}
//	}
//	this->num_keys--;//키의 수를 줄여줌
//}
//
//template<typename E>
//void Trie<E>::eraseTrie()
//{
//	TrieNode<E>* pTN;
//	TrieNode<E>* pTN_to_be_deleted = NULL;
//	if (this->_root == NULL)//삭제대상 없음
//		return;
//	pTN = this->_root;
//	/* delete the last key word first */
//	while (pTN != NULL)//삭제대상이 있을때 반복
//	{
//		while ((pTN != NULL) && (pTN->getNext()))//다음노드가 있을경우
//			pTN = pTN->getNext();//다음위치의 끝으로이동
//		while (pTN->getChild())//자식이 존재시
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
// 손용준 선배 거

//#ifndef Trie_H
//#define Trie_H
//#include "TrieNode.h"
//#define MAX_STR_LEN 50
//#define ISITRIGHT
//using namespace std;
//
//typedef list<MyVoca*> List_pVoca; //마이보카형_리스트
//typedef list<MyVoca*>::iterator List_pVoca_Iter; //리스트 반복자
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
//	void deleteKeyStr(string keyStr); //한 키워드 삭제
//	void eraseTrie(); //트라이 전체 삭제
//	void fprintTrie(ostream& fout);
//
//protected:
//	TrieNode<E>* _find(string keyStr, SearchMode sm = FULL_MATCH);
//	void _traverse(TrieNode<E>* pTN, List_pVoca& list_keywords);
//
//private:
//	TrieNode<E>* _root;
//	int num_keys; //한 키워드(단어)에 대한거임
//	string trie_name;
//};
//
///*구현부*/
///*생성자*/
//template<typename E>
//Trie<E>::Trie(string name)
//{
//	trie_name = name;
//
//	/*루트 설정*/
//	_root = new TrieNode<E>('\0', NULL);
//	_root->setKey('\0');
//	_root->setPrev(NULL);
//	_root->setNext(NULL);
//	_root->setParent(NULL);
//	_root->setChild(NULL);
//	num_keys = 0;
//}
//
///*한단어 끝을 알리는 Trie Node 삽입*/
///*pTN 까지가 한단어고, 얘는 그 밑에 붙어 한단어 임을 알림
//->키는 '\0', VALUE를 가짐*/
///*밑 인서트 내부 구현으로 쓰임*/
//template<typename E>
//void Trie<E>::insertExternalTN(TrieNode<E>* pTN, string keyStr, E value)
//{
//	TrieNode<E>* pTN_New = NULL;
//	pTN_New = new TrieNode<E>('\0', value); //노드 생성
//
//	pTN->setChild(pTN_New);
//	(pTN->getChild())->setParent(pTN);
//	pTN_New->setValue(value); //위 생성에서 애초에 했는데 이건 왜하냐
//}
//
///*insert(keyword, value) to Trie*/
//template<typename E>
//void Trie<E>::insert(string keyStr, E value)
//{
//	TrieNode<E>* pTN = NULL, * pTN_New = NULL;
//
//	/*string 내부 함수임, 반환형이 char**/
//	char* keyPtr = (char*)keyStr.c_str(); //스트링을 C스타일의 문자열로 바꾸는 함수임
//
//	/*키워드(단어)가 없었던 거임*/ /*keyPtr이란 문자열임*/ /* *keyPtr은 문자임 */
//	if (keyPtr == NULL)
//		return;
//
//	/*풀매치 모드로 키워드 탐색*/ /*없을때 널 반환*/
//	/*이미 트라이에 있는 키워드(문자열) 인거임*/
//	if (_find(keyStr, FULL_MATCH) != NULL)
//	{
//		cout << "The given key string (" << keyStr << ") is already existing; just return." << endl;
//		return;
//	}
//
//	/*루트부터*/
//	pTN = this->_root;
//
//	/*삽입 위치 찾는거임*/
//	/*pTN이 존재하고, 삽입할 키워드(문자열)가 끝나지 않은 동안*/
//	while ((pTN != NULL) && (*keyPtr != '\0'))
//	{
//		/*pTN의 키(문자)보다 삽입할 키(문자)가 더 크고, */
//		/*pTN의 오른쪽이 없다면 while 종료. 거따 넣으면 됨*/
//		if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))
//			break;
//
//		/*pTN의 키(문자)보다 삽입할 키(문자)가 더 크고,*/
//		/*오른쪽이 존재한다면 오른쪽으로 넘어가 계속*/
//		while ((pTN->getKey() < *keyPtr) && (pTN->getNext() != NULL))
//			pTN = pTN->getNext();
//
//		/*트라이에 있는 키(문자)임. 그 단어 타고 내려감*/
//		/*삽입할 키워드(단어)가 끝나지 않은동안*/
//		/*그럼 경우는 세가지 나옴.*/
//		/*1)기존 단어보다 작은 2)기존단어보다 큰 3) 기존단어 끝까지 안가고 중간에 세는*/
//		while ((pTN != NULL) && (pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))
//		{
//			pTN = pTN->getChild();
//			keyPtr++;
//		}
//
//		/*만약 삽입할 키워드(단어)는 안끝났고, pTN위치의 키(문자)가 더 크다면 탈출*/
//		/*예를들어 stance가 있었는데, stack을 넣음*/
//		if ((pTN->getKey() > *keyPtr) && (*keyPtr != '\0'))
//			break;
//	} //삽입 시작할 위치 잡은거임 //end of while
//
//
//	/*하나*/
//	/*자 위의 1)경우임. there was a longer key string already*/
//	/*action 있는데 act 넣는거임*/
//	if ((pTN->getKey() != '\0') && (*keyPtr == '\0'))
//	{
//		/* break the longer key string, and connected to the separated key strings */
//		pTN_New = new TrieNode<E>('\0', value); //external node
//		pTN_New->setParent(pTN->getParent());
//		(pTN->getParent())->setChild(pTN_New); //action의 i가 *pTN임
//		pTN_New->setNext(pTN);
//		pTN->setPrev(pTN_New);
//
//		//cout << "key (" << keyWord << ") is inserted" << endl;
//		this->num_keys++; //한 키워드(단어)에 대해네
//		return;
//	}
//
//	/*둘*/
//	/*aged 있었고, agenda 넣는 케이스. pTN: d*/
//	else if ((pTN->getKey() < *keyPtr) && (pTN->getNext() == NULL) && (*keyPtr != '\0'))
//	{
//		pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE); //'n'임
//		pTN_New->setParent(pTN->getParent());
//		pTN_New->setPrev(pTN);
//		pTN->setNext(pTN_New);
//
//		pTN = pTN_New; //'n'
//		keyPtr++;
//		while (*keyPtr != '\0') //삽입할 키워드(단어)의 끝까지
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE); //새 노드 만듬
//			pTN->setChild(pTN_New);
//			(pTN->getChild())->setParent(pTN); //걍 pTN_New의 부모설정이라 하면 될걸 굳이
//
//			pTN = pTN->getChild(); //옮겨가서 다시 수행
//			keyPtr++;
//		}
//
//		/*삽입할거 다함. pTN:agenda의 마지막 'a'*/
//		if (*keyPtr == '\0')
//		{
//			insertExternalTN(pTN, keyStr, value);
//			this->num_keys++;
//			return;
//		}
//	}
//
//	/*셋*/
//	/*stance 있었는데 stack 넣는거임. pTN:stance의 'n'*/
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
//		/*이건 뭐 star랑 stack있었는데 stance 넣는거. pTN:star의 'r'*/
//		else  //pTN_New :stance의 'n'
//		{
//			(pTN->getPrev())->setNext(pTN_New);
//		}
//
//		pTN_New->setPrev(pTN->getPrev());
//		pTN->setPrev(pTN_New);
//		pTN = pTN_New;
//		keyPtr++;
//
//		/*삽입할 키워드(단어) 끝까지 넣음*/
//		while (*keyPtr != '\0')
//		{
//			pTN_New = new TrieNode<E>(*keyPtr, VALUE_INTERNAL_NODE);
//			pTN->setChild(pTN_New);
//			(pTN->getChild())->setParent(pTN);
//			pTN = pTN->getChild();
//			keyPtr++;
//		}
//
//		/*삽입할 키워드(단어)의 끝*/
//		if (*keyPtr == '\0')
//		{
//			insertExternalTN(pTN, keyStr, value);
//			this->num_keys++;
//			return;
//		}
//	}
//}
//
///*find(keyWord) -> 노드 포인터 반환*/
//template<typename E>
//TrieNode<E>* Trie<E>::find(string keyWord)
//{
//	TrieNode<E>* pTN = NULL;
//	pTN = _find(keyWord, FULL_MATCH);
//	return pTN;
//}
//
///*_find(keyWord(단어), SearchMode) -> 노드 포인터 반환*/
////풀이면 해당 단어의 끝, 프레면 서치키워드의 끝을 보냄
//template<typename E>
//TrieNode<E>* Trie<E>::_find(string keyStr, SearchMode sm)
//{
//	/*string을 C스타일 문자열로 바꾸는 함수*/
//	const char* keyPtr = keyStr.c_str();
//	TrieNode<E>* pTN = NULL;
//	TrieNode<E>* found = NULL;
//
//	/*키워드가 없음*/ /*keyPtr이란 문자열임*/
//	if (keyPtr == NULL)
//		return NULL;
//
//	/*루트부터 검색*/
//	pTN = this->_root;
//
//	/*더이상 노드가 없거나, 서치키워드가 이제 '\0'일때 탈출*/
//	/* *keyPtr이란 문자를 나타냄*/
//	while ((pTN != NULL) && (*keyPtr != '\0'))
//	{
//		/*pTN이 있고, pTN의 키(문자)가 찾는 키(문자)보다 작은동안 오른쪽으로*/
//		/*이러면 서치키(문자)와 같거나,*/
//		/*서치키(문자)가 없으면 더 큰 노드(문자)로 옮겨감 pTN이*/
//		while ((pTN != NULL) && (pTN->getKey() < *keyPtr))
//		{
//			if (pTN->getNext() == NULL)
//				return NULL;
//			pTN = pTN->getNext();
//		}
//
//		/*pTN은 있는데, 그게 서치키(문자)랑 다른거*/
//		if ((pTN != NULL) && (pTN->getKey() > *keyPtr))
//		{
//			return NULL; //못찾은거임
//		}
//
//		/*오른쪽으로 옮겨옴 pTN이 없는거임, 서치키워드도 안끝났고*/
//		else if ((pTN == NULL) && (*keyPtr != '\0'))
//		{
//			return NULL; //못찾은거임
//		}
//
//		/*pTN의 키(문자)와 서치키(문자)가 동일함, 서치키워드(단어)도 안끝났고*/
//		else if ((pTN->getKey() == *keyPtr) && (*keyPtr != '\0'))
//		{
//			pTN = pTN->getChild(); //자식으로 가서
//			keyPtr++; //다음키(문자)로 감
//
//			/*서치키워드(단어) 끝났단 소리임*/
//			if (*keyPtr == '\0')
//			{
//				/* key or prefix found*/
//				if (sm == FULL_MATCH)
//				{
//					/*pTN자식으로 왔더니 한단어의 풀매치로 다 찾은거임*/
//					if (pTN->getKey() == '\0')
//					{
//						return pTN;
//					}
//					/*pTN의 자식으로 왔는데, 트라이 내 단어는 안끝났음. 풀매치 아님, 즉, 못찾은거 처리임*/
//					else //(pTN->getKey() != '\0')
//					{
//						return NULL;
//					}
//				}
//
//				/*풀매치 아님 걍 찾았음 된거임*/
//				/*걍 서치키워드가 다 포함돼있으면 그걸로 된거임*/
//				else if (sm == PREFIX_MATCH)
//				{
//					return pTN;
//				}
//			}
//
//			/*자식으로 왔더니*/
//			/*트라이 내 한단어는 끝인데 서치키워드(단어)는 안끝났단 소리임*/
//			/*예를들어 action 찾을때, act의 '\0'까지 온거임*/
//			else if ((pTN->getKey() == '\0') && (*keyPtr != '\0'))
//			{
//				if (pTN->getNext() != NULL) //옆으로 옮겨가서 다시 돌려
//				{
//					pTN = pTN->getNext();
//					continue;
//				}
//				else
//					return NULL;
//			}
//
//			/*트라이 내 한단어 끝이 아니었으니 다시 내려가*/
//			else
//				continue;
//		}
//	}
//}
//
///*순회-> 마이보카.포인터.형_리스트에 담음*/
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
//	/*해당 프레픽스 같이 갖는 단어들임*/
//	if (pTN->getNext() != NULL)
//	{
//		_traverse(pTN->getNext(), list_keywords);
//	}
//}
//
///*접두로 탐색*/
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
//	pTN = this->_root; //트라이 루트부터
//	pTN = _find(keyStr, PREFIX_MATCH);
//
//	/*싹 주워담아!!*/
//	_traverse(pTN, predictWords);
//}
//
//
///*키워드(단어) 삭제*/
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
//	pTN = _find(keyWord, FULL_MATCH); //풀이면 해당 단어의 끝(external node), 프레면 서치키워드의 끝을 보냄
//	if (pTN == NULL)
//	{
//		cout << "Key [" << keyWord << "] not found in trie" << endl;
//		return;
//	}
//
//	while (1)
//	{
//		/*삭제할 키워드(단어) 다 삭제함*/
//		if (pTN == NULL)
//			break;
//#ifndef ISITRIGHT
//		/*있고, 있고. 예로 star 삭제 한다보셈. star의 extern 앞에도 뭐 있다 생각하셈. pTN: extern nod*/
//		if (pTN->getPrev() && pTN->getNext())
//		{
//			tmp = pTN;
//			(pTN->getNext())->setPrev(pTN->getPrev());
//			(pTN->getPrev())->setNext(pTN->getNext());
//
//			free(tmp); //노드 삭제
//			break; //break??? what about r?? 아! stare는 남아 있네
//		}
//
//		/*있고, 없고. 예로 stare 보고, extern 왼쪽에 뭐있다 생각하셈.*/
//		/*stare 보면 안되겠다. stare는 애초 없다하고, star 삭제한다 보셈. extern 왼쪽에 뭐 있고*/
//		/*아 이것도 안되네 애초에 extern 왼쪽에 존재할수가 있냐?*/
//		/*아니 정렬순이라 애초에 extern 왼편에는 존재할수가 없지 않냐?*/
//		else if (pTN->getPrev() && !(pTN->getNext()))
//		{
//			tmp = pTN;
//			(pTN->getPrev())->setNext(NULL);
//
//			free(tmp); //extern 만 삭제 되는거임. 
//			break;
//		}
//#endif		
//		int p = 0; //이건 위에 if 사라졌을시를 위해 있는 더미임
//		if (p == 1);
//
//		/*없있. 예로 act 삭제. 암튼 오른편에 있단건 이어지는게 있단거니까 위에 다 지울필요 없음*/
//		else if (!(pTN->getPrev()) && pTN->getNext())
//		{
//			tmp = pTN;
//
//			(pTN->getParent())->setChild(pTN->getNext());
//			pTN = pTN->getNext();
//			pTN->setPrev(NULL);
//
//			free(tmp); //extern nod삭제
//			break;
//		}
//
//		/*없없. 위에도 다 지워줘야 함. act랑 cat 만있었고, act 지운다 생각하셈.*/
//		else
//		{ //pTN은 external node
//			tmp = pTN;
//			pTN = pTN->getParent(); //위로. while때문에 어차피 계속 돌거임
//
//			if (pTN != NULL) //아래에서 위로 지워나갈거니까
//				pTN->setChild(NULL);
//
//			free(tmp);
//
//			/*루트까지 온거임*/ /*트라이 젤 첫 노드의 prev임*/
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
///*트라이 전체 삭제*/
//template<typename E>
//void Trie<E>::eraseTrie()
//{
//	TrieNode<E>* pTN;
//	TrieNode<E>* pTN_to_be_deleted = NULL;
//
//	if (this->_root == NULL) //루트도 없는겨
//		return;
//
//	pTN = this->_root; //루트부터 탐색
//
//	/* delete the last key word first */
//	while (pTN != NULL) //걍 다 삭제되기 전까지 계속 돌아
//	{
//		while ((pTN != NULL) && (pTN->getNext())) //계속 오른쪽으로
//			pTN = pTN->getNext();
//
//		while (pTN->getChild()) //계속 아래로
//		{
//			if (pTN->getNext()) //만약 오른쪽에 있음 잠깐 멈추고
//				break;
//			pTN = pTN->getChild();
//		}
//
//		if (pTN->getNext()) //오른쪽으로 간다음 다시 위 while실행해 
//			continue;
//		/*그럼 이제 최하단 최우측 도착함*/
//
//
//		/*걍 위에서 힘들게 구현한 delete 쓰면 안되냐 ㅠ*/
//		/*아 저게 반환이 없어서 연계가 안되겠다. 담아두고 하면 되지?*/
//		/*근데 이건 next가 있을수 있냐? 없음. 오른쪽 있는 쪽 구현은 지워도 됨*/
//		/*있있*/
//		if (pTN->getPrev() && pTN->getNext())
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getNext())->setPrev(pTN->getPrev());
//			(pTN->getPrev())->setNext(pTN->getNext());
//			pTN = pTN->getNext();
//			free(pTN_to_be_deleted);
//		}
//
//		/*있없*/
//		else if (pTN->getPrev() && !(pTN->getNext()))
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getPrev())->setNext(NULL);
//			pTN = pTN->getPrev();
//			free(pTN_to_be_deleted);
//		}
//
//		/*없있*/
//		/*else if (!(pTN->getPrev()) && pTN->getNext())
//		{
//			pTN_to_be_deleted = pTN;
//			(pTN->getParent())->setChild(pTN->getNext());
//			(pTN->getNext())->setPrev(NULL);
//			pTN = pTN->getNext();
//			free(pTN_to_be_deleted);
//		}*/
//
//		/*없없*/
//		else
//		{
//			pTN_to_be_deleted = pTN;
//			if (pTN == this->_root)
//			{
//				this->num_keys = 0; //안해도 될거 같은데 안전빵인가 어차피 삭제하며 지우잖아..? 안하네 위에서 아무도
//				return;
//			}
//			if (pTN->getParent() != NULL)
//			{
//				pTN = pTN->getParent();
//				pTN->setChild(NULL); //아래서 위로 삭제하니까
//			}
//			else //이제 부모없음
//			{
//				pTN = pTN->getPrev();
//			}
//
//			free(pTN_to_be_deleted);
//		}
//	} //end while
//}
//
///*트라이 출력*/
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