#include "Imagen.h"

#include <string.h>

// Tamaño de las imágenes en pixels
const unsigned int Imagen::NUM_PIXELS = 1000;

//------------------------------------------------------------

Imagen::Imagen()
{
	_data = new int[NUM_PIXELS * 3];
	memset(_data, 0, NUM_PIXELS*3*sizeof(int));
}

//------------------------------------------------------------

Imagen::Imagen(const Imagen &imagen)
{
	_data = new int[NUM_PIXELS * 3];
	memcpy(_data, imagen._data, NUM_PIXELS*3*sizeof(int));
}

//------------------------------------------------------------

Imagen::~Imagen()
{
	delete[] _data;
	_data = NULL;
}

//------------------------------------------------------------

Imagen& Imagen::operator=(const Imagen &imagen)
{
	memcpy(_data, imagen._data, NUM_PIXELS*3*sizeof(int));

	return *this;
}

//------------------------------------------------------------

Imagen& Imagen::operator++()
{
	for (unsigned int i=0; i<NUM_PIXELS*3; ++i)
		++_data[i];

	return *this;
}

//------------------------------------------------------------

Imagen Imagen::operator++(int)
{
	Imagen aux(*this);
	++(*this);
	return aux;
}

//------------------------------------------------------------

void Imagen::swap(Imagen &imagen)
{
	int *aux = _data;
	_data = imagen._data;
	imagen._data = aux;
}

	


	