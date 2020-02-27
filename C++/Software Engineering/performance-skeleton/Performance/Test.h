#pragma once

#include <string>

/* Clase abstracta de la que deben heredar los tests. */
class Test
{
public:
	/* Constructor. Recibe el nombre del test. */
	Test(const std::string &name);

	/* Destructor. */
	virtual ~Test();

	/* Devuelve el nombre del test. */
	std::string getName();

	/* Ejecuta el test. Se debe implementar en cada test concreto. */
	virtual void run() = 0;

private:
	// Nombre del test.
	std::string _name;
};

