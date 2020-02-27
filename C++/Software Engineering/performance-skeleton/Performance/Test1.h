#pragma once

#include "Test.h"
#include "TestRunner.h"
#include <stdio.h>
#include <iostream>

class Test1A : public Test
{
public:
	Test1A(const std::string& name, int n) : Test(name), iterations(n){ };

	virtual ~Test1A() { };

	void run()
	{
		for (unsigned int i = 0; i < iterations; ++i) {
			printf("Hello World\n");
		}
	};

private:
	int iterations;
};

class Test1B : public Test
{
public:
	Test1B(const std::string& name, int n) : Test(name), iterations(n) { };

	virtual ~Test1B() { };

	void run()
	{
		for (unsigned int i = 0; i < iterations; ++i) {
			std::cout << "Hello World\n";
		}
	};

private:
	int iterations;
};

class TestRunner1 : public TestRunner
{
public:
	TestRunner1() : TestRunner()
	{
		addTest(new Test1B("cout", 1000));
		addTest(new Test1A("printf", 1000));
	}

	virtual ~TestRunner1() { };
};