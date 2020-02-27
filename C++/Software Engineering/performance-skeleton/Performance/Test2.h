#pragma once

#include "Test.h"
#include "TestRunner.h"
#include <stdio.h>
#include <iostream>

class Test2A : public Test
{
public:
	Test2A(const std::string& name, int fact) : Test(name), fact(fact) { };

	virtual ~Test2A() { };

	void run()
	{
		for (int i = 0; i < 1000; ++i)
		{
			Factorial(fact);
		}
	};

private:
	int fact;
	int Factorial(int n)
	{

		auto max = n < 1 ? 1 : n;
		int buff = 1;
		for (int i = 1; i <= max; i++)
		{
			buff *= i;
		}
		return buff;
	}
};

class Test2B : public Test
{
public:
	Test2B(const std::string& name, int fact) : Test(name), fact(fact) { };

	virtual ~Test2B() { };

	void run()
	{
		for (int i = 0; i < 1000; ++i)
		{
			Factorial(fact);
		}
	};

private:
	int fact;

	int Factorial(int n)
	{
		if (n < 1)
		{
			return 1;
		}
		else
		{
			return n * Factorial(n - 1);
		}

	}
};

class TestRunner2 : public TestRunner
{
public:
	TestRunner2() : TestRunner()
	{
		addTest(new Test2B("Recursive", 1000));
		addTest(new Test2A("Iterative", 1000));
	}

	virtual ~TestRunner2() { };
};
