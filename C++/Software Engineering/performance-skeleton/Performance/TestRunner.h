#pragma once

#include "Test.h"

#include <vector>

/* Ejecuta tests y muestra los tiempos. */
class TestRunner
{
public:
	/* Constructor por defecto. */
	TestRunner();

	/* Destructor. */
	virtual ~TestRunner();

	/* Añade un nuevo test. Se hace cargo de liberar la memoria del test al finalizar. */
	void addTest(Test *test);

	/* Elimina todos los tests. */
	void clear();

	/* Ejecuta los test y muestra los tiempos de ejecución. */
	void run();

private:
	// tests
	std::vector<Test *> _tests;
};

