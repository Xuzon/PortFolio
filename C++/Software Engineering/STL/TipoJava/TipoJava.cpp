#include <iostream>
#include <stdio.h>
#include <assert.h>

class Object {
public:
	virtual bool operator<(const Object&) const = 0;

};

//#define MAX_SIZE 1000

class JavaTypeContainer 
{

public:
	JavaTypeContainer() : size(0) {}
	~JavaTypeContainer() {}

	//GUARDO PUNTERO A OBJETO, SI LO LIBERAS REVIENTA
	void PushBack(Object &toAdd)
	{
		assert(size < MAX_SIZE);
		data[size] = &toAdd;
		++size;
	}

	void PopBack()
	{
		--size;
	}

	const Object& operator[](unsigned int index) const
	{
		return *data[index];
	}
	Object& operator[](unsigned int index)
	{
		return *data[index];
	}

	unsigned int Size() const
	{
		return size;
	}

private:
	//const  static unsigned int MAX_SIZE = 1000;
	enum
	{
		MAX_SIZE = 1000
	};
	unsigned int size;
	Object *data[MAX_SIZE];
};

class ClassCTJ : public Object
{
public:
	int a;
	bool operator<(const Object& other) const
	{
		const ClassCTJ& otherctj = (const ClassCTJ&)other;
		return a < otherctj.a;
	}
};

bool IsSorted(const JavaTypeContainer& container, unsigned int init, unsigned int final)
{
	assert(init <= final);
	for (int i = init; i < final - 1; ++i)
	{
		if (container[i + 1] < container[i])
		{
			return false;
		}
	}
	return true;
}

int main()
{
	JavaTypeContainer container;
	ClassCTJ mc1, mc2, mc3;
	mc1.a = 1;
	mc2.a = 2;
	container.PushBack(mc1);
	container.PushBack(mc2);
	mc3 = (ClassCTJ&)container[1];

	std::cout << mc3.a << "\n";

	if (IsSorted(container, 0, container.Size()))
	{
		std::cout << "Esta ordenado\n";
	}
}