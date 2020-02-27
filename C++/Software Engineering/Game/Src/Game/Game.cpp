#include <iostream>
#include <windows.h>
#include "GraphicsEngine/GraphicsEngine.h"

//puntero a una funcion que devuelve void y recibe un const char*
using fSaluda = void (*)(const char*);

extern "C"
{
	__declspec(dllimport)void saluda(const char*);
}

int main()
{

	saluda("Dani");
	/*HINSTANCE dll = LoadLibrary(L"DLL");

	if (dll == NULL)
	{
		std::cout << "The dll couldn't be loaded :(\n";
		return 0;
	}

	fSaluda saluda = (fSaluda)GetProcAddress(dll, "saluda");

	if (saluda == NULL)
	{
		std::cout << "Saluda didn't found\n";
		FreeLibrary(dll);
		return 2;
	}

	saluda("Dani");*/

	//Hello("Dani");
	return 0;
}