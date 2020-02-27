#include "TestRunner.h"

#include <time.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

//------------------------------------------------------------

TestRunner::TestRunner()
{

}

//------------------------------------------------------------

TestRunner::~TestRunner()
{
	clear();
}

//------------------------------------------------------------

void TestRunner::addTest(Test *test)
{
	_tests.push_back(test);
}

//------------------------------------------------------------

void TestRunner::clear()
{
	vector<Test *>::iterator it = _tests.begin();
	while (it != _tests.end()) {
		delete *it;
		it++;
	}
	_tests.clear();
}

//------------------------------------------------------------

void TestRunner::run()
{
	// vector de tiempos de ejecución
	vector<double> times(_tests.size());

	// Ejecutar los test y anotar tiempos
	for (unsigned int i=0; i<_tests.size(); ++i) {
		clock_t start = clock();
		_tests[i]->run();
		clock_t end = clock();

		times[i] = end - start;
	}

	// Mostrar tiempos
	cout << endl;
	cout << "Tiempos:" << endl;
	for (unsigned int i=0; i<_tests.size(); ++i) {
		cout << _tests[i]->getName() << ": " << fixed << setprecision(2) << times[i] << endl;
	}


	// Normalizar los tiempos
	double min  = *std::min_element(times.begin(), times.end());
	for (unsigned int i=0; i<_tests.size(); ++i)
		times[i] /= min;

	// Mostrar tiempos normalizados
	cout << endl;
	cout << "Tiempos normalizados:" << endl;
	for (unsigned int i=0; i<_tests.size(); ++i) {
		cout << _tests[i]->getName() << ": " << fixed << setprecision(2) << times[i] << endl;
	}
}

//------------------------------------------------------------