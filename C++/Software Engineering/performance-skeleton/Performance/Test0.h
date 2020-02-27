/*
 * Tiempo que se tarda en crear arrays de distintos tipos.
 * Tipos: int, std::complex<double>, std::string, Imagen, Imagen*
 */

#pragma once

#include "Test.h"
#include "TestRunner.h"
#include "Imagen.h"

#include <complex>
#include <string>

static const unsigned int N0 =  10 * 1000;

//------------------------------------------------------------

template<class T, int N>
class Test0 : public Test
{
public:
	Test0(const std::string &name) : Test(name) { };
	
	virtual ~Test0() { };
	
	void run()
	{
		for (unsigned int i=0; i<N0; ++i) {
			T aux[N];
		}
	};
};

//------------------------------------------------------------

class TestRunner0 : public TestRunner
{
public:
	TestRunner0() : TestRunner()
	{
		addTest(new Test0<int, 100>("int 100"));
		addTest(new Test0<std::complex<double>, 100>("complex 100"));
		addTest(new Test0<std::string, 100>("string 100"));
		addTest(new Test0<Imagen, 100>("imagen 100"));
		addTest(new Test0<Imagen *, 100>("imagen* 100"));
	}

	virtual ~TestRunner0() { };
};

//------------------------------------------------------------