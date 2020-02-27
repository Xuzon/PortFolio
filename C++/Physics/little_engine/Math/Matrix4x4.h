/*----------------------------------------------------------------------------------------------------------------------*/
// Fichero Matrix4x4.h
// Cabecera de la clase que implementa una matriz para manejo de gráficos 3D de floats. Optimizado con SSE
// By Iñaki Hernández Gastañaga (2007) (Siguiendo directrices de 3D Game Engine Programming).
/*----------------------------------------------------------------------------------------------------------------------*/
#ifndef _MATRIX_4x4_
#define _MATRIX_4x4_ 

// Includes
#include "Vector3D.h"
#include "Vertex.h"

//@TODO: Analizar las funciones que no tengan comentario, y añadirselo
// Estructura para las matrices basicas
struct SIMPLE_MATRIX
{
	float v[16];
};

// Definición de la clase
class __declspec( dllexport ) CMatrix4x4
{
public:
  union{
		struct{
	  float	_11, _12, _13, _14,
			    _21, _22, _23, _24,
			    _31, _32, _33, _34,
			    _41, _42, _43, _44;
		};
		float v[16];
	};

    // auxiliar para optimizar multiplicaciones
  mutable float m_x, m_y, m_z, m_w;

	/**
	 * Constructor por defecto
	 */
	CMatrix4x4( void ) {}

	/**
	 * Carga la matriz identidad
	 */
	inline void Identity(void);                       

	/**
	 * Carga una matriz de rotación alrededor del eje X del mundo
	 */
	inline void RotaX(float a);                       

	/**
	 * Carga una matriz de rotación alrededor del eje Y del mundo
	 */
	inline void RotaY(float a);                       

	/**
	 * Carga una matriz de rotación alrededor del eje Z del mundo
	 */
	inline void RotaZ(float a);                       

	/**
	 * 
	 */
	inline void Rota(const CVector3D &vc);            // x, y and z

	/**
	 * 
	 */
	inline void Rota(float x, float y, float z);      // x, y and z
	inline void RotaZXY(float x, float y, float z);      // x, y and z
	//inline void RotaXZY(float x, float y, float z);

	/**
	 * Carga una matriz de rotación alrededor de un eje pasado como parámetro
	 * @param eje de rotación
	 * @param angulo a rotar
	 */
	inline void RotaArbi(const CVector3D &vcAxis, float a);

	/**
	* Carga una matriz de rotación alrededor de un eje pasado como parámetro
	* @param eje de rotación
	* @param angulo a rotar
	*/
	inline void BuildMatrixFromAxisAngle( const CVector3D &vcAxis, float a );

	/**
	 * 
	 */
	inline void ApplyInverseRota(CVector3D *pvc);

	/**
	 * Añade los valores de traslación pasados a los correspondientes en la matriz
	 */
	inline void Translate(float dx, float dy, float dz);

	/**
	 * 
	 */
	inline void SetTranslation(CVector3D vc, bool EraseContent=false);

	/**
	 * 
	 */
	inline CVector3D GetTranslation(void);


	/**
	 * 
	 */
	inline CVector3D GetEulerAngles();

	/**
	* 
	*/
	inline void Billboard(CVector3D vcPos, CVector3D vcDir,
		CVector3D vcWorldUp = CVector3D(0,1,0));

	/**
	* 
	*/
	inline void LookAt(CVector3D vcPos, CVector3D vcLookAt, 
		CVector3D vcWorldUp = CVector3D(0,1,0));

	/**
	* Carga la matriz transpuesta a la que se le pasa como parámetro
	* @param m Matriz a parti de la cual se debe calcular una matriz transpuesta a ella
	*/
	inline void TransposeOf( const CMatrix4x4 &m );       

	/**
	* Carga la matriz inversa a la que se le pasa como parámetro
	* @param m Matriz a partir de la cual se debe calcular una matriz inversa a ella
	*/
	inline void InverseOf( const CMatrix4x4 &m );         

	/**
	* Multiplicación de matrices
	*/
	CMatrix4x4 operator * ( const CMatrix4x4 &m )const;    

	/**
	* Multiplicación matriz, vector
	*/
	CVector3D operator * ( const CVector3D &vc )const; 

	/**
	* Multiplicación matriz, vértice
	*/
	SIMPLE_VERTEX operator * ( const SIMPLE_VERTEX &vc ) const; 

	/**
	* Multiplicaciones mas eficientes
	*/
	inline void Multiply( SIMPLE_VERTEX &vc ) const {
		m_x = vc.x;
		m_y = vc.y;
		m_z = vc.z;
		vc.x = m_x * _11 + m_y * _21 + m_z * _31 + _41;
		vc.y = m_x * _12 + m_y * _22 + m_z * _32 + _42;
		vc.z = m_x * _13 + m_y * _23 + m_z * _33 + _43;
	}
	
	/**
	* Multiplicaciones mas eficientes
	*/
	inline void Multiply( const SIMPLE_VERTEX &inVector, SIMPLE_VERTEX &outVector ){
		outVector.x = inVector.x * _11 + inVector.y * _21 + inVector.z * _31 + _41;
		outVector.y = inVector.x * _12 + inVector.y * _22 + inVector.z * _32 + _42;
		outVector.z = inVector.x * _13 + inVector.y * _23 + inVector.z * _33 + _43;
	}

	// Rellena una simpleMatrix
	void FillSM( SIMPLE_MATRIX *sm ) const {
		//sm->v[0] = v[0];
		//sm->v[1] = v[4];
		//sm->v[2] = v[8];
		//sm->v[3] = v[12];
		//sm->v[4] = v[1];
		//sm->v[5] = v[5];
		//sm->v[6] = v[9];
		//sm->v[7] = v[13];
		//sm->v[8] = v[2];
		//sm->v[9] = v[6];
		//sm->v[10] = v[10];
		//sm->v[11] = v[14];
		//sm->v[12] = v[3];
		//sm->v[13] = v[7];
		//sm->v[14] = v[11];
		//sm->v[15] = v[15];
		for( unsigned int i=0; i<16; ++i ){
			sm->v[i] = v[i];
		}
	}
}; 

#endif
