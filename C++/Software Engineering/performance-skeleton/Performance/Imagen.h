#pragma once

/* Imagen de tamaño fijo. Sirve como ejemplo de clase "pesada". */
class Imagen
{
public:
	/* Constructor por defecto. */
	Imagen();

	/* Constructor por copia. */
	Imagen(const Imagen &imagen);

	/* Destructor. */
	virtual ~Imagen();

	/* Operador de asignación. */
	Imagen& operator=(const Imagen &imagen);
	
	/* Operador de pre-incremento. */
	Imagen& operator++(); 

	/* Operador de post-incremento. */
	Imagen operator++(int); 

	/* Intercambia el contenido de dos imágenes. */
	void swap(Imagen &imagen);

private:
	/* Vector de pixels, para simplificar cada pixel son 3 enteros. */
	int *_data;

	/* Número de pixels, para simplificar todas las imágenes tienen el mismo tamaño. */
	static const unsigned int NUM_PIXELS;
};

