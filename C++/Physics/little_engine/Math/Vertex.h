/*----------------------------------------------------------------------------------------------------------------------*/
// Fichero Vertex.h
// Cabecera que define estructuras y constantes para vértices
// By Iñaki Hernández Gastañaga 
/*----------------------------------------------------------------------------------------------------------------------*/
#ifndef _VERTEX_H_
#define _VERTEX_H_

// Forwards
class CVector3D;

// Estructura para los vertices básicos
struct SIMPLE_VERTEX
{
  float x,
        y,  
        z;
};
struct VERTEX_ATTRIB4_FLOAT
{
	float x,
		y,  
		z,
		w;
};
struct VERTEX_ATTRIB4_UINT
{
	unsigned int x,
		y,  
		z,
		w;
};
void Add( SIMPLE_VERTEX &vertex1, const SIMPLE_VERTEX &vertex2 );
void NormalizeVertex( SIMPLE_VERTEX &vertex );
void VertexDifference( const SIMPLE_VERTEX &vertex1, const SIMPLE_VERTEX &vertex2, SIMPLE_VERTEX &resul );
void VertexDifference( const SIMPLE_VERTEX &vertex1, const SIMPLE_VERTEX &vertex2, CVector3D &resul );
float VertexDistance( const SIMPLE_VERTEX &vertex1, const SIMPLE_VERTEX &vertex2 );
void Negate( SIMPLE_VERTEX &vertex1 );
void Negate( SIMPLE_VERTEX *vertex1 );
void Multiply( SIMPLE_VERTEX *vertex, float factor );

// Estructura para V4-s
struct SIMPLE_V4
{
	float x,
		  y,  
		  z,
		  w;
};
#endif