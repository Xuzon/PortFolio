#ifndef _MESH_H_ 
#define _MESH_H_

// Includes
#include "Math/Matrix4x4.h"

// Forwards
struct SIMPLE_VERTEX;
struct VERTEX_ATTRIB4_FLOAT;
class CPhysicsWorld;
namespace physx
{
	class PxActor;
};

// Definicion de la clase
class CMesh
{
public:
	CMesh( const char *geometryFile=0 );
	CMesh( const CVector3D &size );
	virtual ~CMesh( void );
	void Render( void );
	void CreateCube( const CVector3D &size );
	void SetPhysicsActor( physx::PxActor *actor ) { m_physicsActor = actor; }
protected:

	// Para agilizar la demo...
	friend class CPhysicsWorld;

	// Matriz de transformacion
	CMatrix4x4 m_transformMatrix;

	// Info del shape de fisicas
	physx::PxActor *m_physicsActor;

	// info de geometria
	SIMPLE_VERTEX *m_renderVertices;
	SIMPLE_VERTEX *m_transformedVertices;
	SIMPLE_VERTEX *m_renderNormals;
	VERTEX_ATTRIB4_FLOAT *m_vertexColor;
	unsigned int *m_renderIndices;
	unsigned int m_numRenderVertices;
	unsigned int m_numRenderIndices;
};

#endif