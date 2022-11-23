#ifndef T_ARRAY__H
#define T_ARRAY__H

#include<iomanip>
#include<iostream>
#include<string>
using namespace std;

template<typename E>
class T_Array
{
public:
	T_Array(int capa, string nm); // constructor
	~T_Array(); // destructor
	int size() { return num_elements; }
	bool isEmpty() { return num_elements == 0; }
	string getName() { return name; }
	E& operator[](int index) { return t_array[index]; }
protected:
	E* t_array;
	int num_elements;
	int capacity;
	string name;
};

template <typename E>
T_Array<E>::T_Array(int capa, string nm)
{
	t_array = (E*)new E[capa];  // 배열 동적 생성

	num_elements = 0;
	capacity = capa;
	name = nm;
}

template <typename E>
T_Array<E>::~T_Array()
{
	if (t_array != NULL)
		delete[] t_array;

}

#endif