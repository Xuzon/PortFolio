#include <iostream>
#include <stdio.h>
#include <assert.h>


template<typename T>
class CPPTypeContainer
{

public:
	CPPTypeContainer() : size(0) {}
	~CPPTypeContainer() {}

	void PushBack(const T& toAdd)
	{
		assert(size < MAX_SIZE);
		data[size] = toAdd;
		++size;
	}

	void PopBack()
	{
		--size;
	}

	const T& operator[](unsigned int index) const
	{
		return data[index];
	}
	T& operator[](unsigned int index)
	{
		return data[index];
	}

	unsigned int Size() const
	{
		return size;
	}

	T* Begin()
	{
		return data;//&data[0]
	}

	T* End()
	{
		return data + size;//&data[size]
	}

	
private:
	//const  static unsigned int MAX_SIZE = 1000;
	enum
	{
		MAX_SIZE = 1000
	};
	unsigned int size;
	T data[MAX_SIZE];
};

class MyClass
{
public:
	int a;
};

class MySubClass : public MyClass
{
public:
	int b;
};

template<typename I>
bool IsSorted(I init, I end)
{
	assert (init <= end);
	if (init == end)
	{
		return true;
	}
	I last = init;
	I current = init;
	++current;
	while (current != end && !(*current < *last))
	{
		++current;
		++last;
	}

	return current == end;
}
//template<typename CONTAINER>
//bool IsSorted(const CONTAINER &container, unsigned int init, unsigned int end)
//{
//	assert(init <= final);
//	for (int i = init; i < end - 1; ++i)
//	{
//		if (container[i + 1] < container[i])
//		{
//			return false;
//		}
//	}
//	return true;
//}

int main()
{
	CPPTypeContainer<MyClass> cont;
	MyClass mcpp1, mcpp2, mcpp3;
	mcpp1.a = 1;
	mcpp2.a = 2;
	cont.PushBack(mcpp1);
	cont.PushBack(mcpp2);
	mcpp3 = cont[1];

	std::cout << mcpp3.a << "\n";

	/*if (IsSorted(cont,0, cont.Size()))
	{
		std::cout << "El contenedor de mi clase esta ordenado\n";
	}*/

	CPPTypeContainer<int> contInts;
	contInts.PushBack(3);
	contInts.PushBack(15);
	int a = contInts[1];

	std::cout << a << "\n";

	if (IsSorted(contInts.Begin(), contInts.End()))
	{
		std::cout << "El contenedor de enteros esta ordenado\n";
	}

	int arr[10];
	arr[0] = 10;
	arr[1] = 20;
	if (IsSorted(arr, arr + 2))
	{
		std::cout << "Array sorted by pointers";
	}
}