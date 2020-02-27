/*----------------------------------------------------------------------------------------------------------------------*/
// Fichero CVector3f.h
// Cabecera de la clase que implementa un vector para manejo de gráficos 3D de floats. Optimizado con SSE
// By Iñaki Hernández Gastañaga (2007) (Siguiendo directrices de 3D Game Engine Programming).
/*----------------------------------------------------------------------------------------------------------------------*/
#ifndef _VECTOR_3D_
#define _VECTOR_3D_

// Includes
#include "Vertex.h"

// Forwards
class CMatrix4x4;

// Definición de clases
class  __declspec( dllexport ) CVector3D
{
public:

	// vaores en las diferentes coordenadas para manejo de 3D
	float x, y, z, w;

	/**
	 * Constructor por defecto
	 */
	CVector3D( void ){ x = y = z = 0.0f; w = 1.0f; }

	/**
	 * Constructor a medida
	 */
	CVector3D( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z ), w( 1.0f ) {}

	/**
  * Constructor a medida
  */
	CVector3D( SIMPLE_VERTEX vertex ) : x( vertex.x ), y( vertex.y ), z( vertex.z ), w( 1.0f ) {}

	/**
	* Obtiene un simple vertex a partir de el vector
	*/
	SIMPLE_VERTEX ToSimpleVertex( void ) const;

	/**
	* Distancia a un vertice
	*/
	float SqrDistanceToVertex( const SIMPLE_VERTEX &vertex ) const;

	/**
	 * Función set para asignar los valores
	 */
	inline void Set( float _x, float _y, float _z ){ x = _x; y = _y; z = _z; w = 1.0f; }

	/**
	 * Nos devuelve la magnitud del vector
	 */
	inline float GetMagnitude( void ) const;

	/**
	 * Nos devuelve la magnitud al cuadrado del vector
	 */
	inline float GetSqrMagnitude( void ) const;

	/**
	 * Niega el vector
	 */
	inline void Negate( void );

	/**
	 * normaliza el vector
	 */
	inline void Normalize( void );

	/**
	 * Nos devuelve el angulo entre la instancia y el vector pasado
	 */
	inline float AngleWith( const CVector3D &v ) const;

	/**
	 * Obtiene el vector desde u a v
	 */
	inline void Difference( const CVector3D &u, const CVector3D &v );

	/**
	 * Asignación
	 */
	void operator= ( const CVector3D &v );

	/**
	 * operador +=
	 */
	void operator+= ( const CVector3D &v );

	/**
	* operador -=
	*/
	void operator-= ( const CVector3D &v );

	/**
	* operador *=
	*/
	void operator*= ( float f );

	/**
	* operador /= con floats
	*/
	void operator/= ( float f );

	/**
	 * operador / con floats
	 */
	CVector3D operator/ ( float f ) const;

	/**
	* operador * con vector
	*/
	float operator* ( const CVector3D &v ) const;

	/**
	* operador * con float
	*/
	CVector3D operator* ( float f ) const;

	/**
	* operador * con matriz
	*/
	CVector3D operator* ( CMatrix4x4 &m ) const;

	/**
	* operador +
	*/
	CVector3D operator+ ( const CVector3D &v ) const;

	/**
	* operador -
	*/
	CVector3D operator- ( const CVector3D &v ) const;

  /**
  * Operador comparacion desigual
  */
  bool operator != ( const CVector3D &other ) const;

  /**
  * Operador comparacion 
  */
  bool operator == ( const CVector3D &other ) const;

	/**
	* Producto vectorial
	*/
	inline void Cross( const CVector3D &u, const CVector3D &v );

	/**
	 * Rota al vector con la matriz pasada, ignorando la información de traslación
	 * de la matriz, si esta está presente
	 */
	inline void RotateWith( const CMatrix4x4 &m );

	/**
	* Rota al vector con la inversa de la matriz pasada, ignorando la información de traslación
	* de la matriz, si esta está presente
	*/
	inline void InvRotateWith( const CMatrix4x4 &m );

	/**
	* Devuelve un nuevo vector con la Y a 0.0f 
	*/
	inline CVector3D CutY() const { return CVector3D(x, 0.0f, z); };

	/**
	* producto escalar
	*/
	static float Dot (const CVector3D& v1, const CVector3D& v2);

	/**
	*
	*/
	static CVector3D SCross( const CVector3D& v1, const CVector3D& v2 );


	/**
	* Distancia al cuadrado entre dos vectores
	*/
	static float SqrDist( const CVector3D& v1, const CVector3D& v2 );

	/**
	* manhattan distance (distancia no diagonal)
	*/
	static float ManhattanDistance(const CVector3D& v1, const CVector3D& v2);
};

#endif