
// Includes
#include "Mesh.h"
#include "Math/Vertex.h"
#include "Math/Vector3D.h"
#include "Math/Quat.h"
#include "stdio.h"
#include "windows.h"
#include "GL/glew.h"

#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "PxDefaultSimulationFilterShader.h"
#include "PxFiltering.h"

// min <= x <= max
__inline float Randf( float min, float max )
{
	return (float)( ((rand()&32767)*(1.0/32767.0))*(max-min) + min );
}
//__inline float Randf( float min, float max )
//{
//	return 100.0f;
//}

#define MAX_GEOM_RND 100.0f 

using namespace physx;

/*----------------------------------------------------------------------------------------------------------------------*/

CMesh::CMesh( const char *geometryFile )
{
	// Creamos un cubo aleatorio
	if( geometryFile ) {
	}

	// Cargamos la geometria
	else {
		CreateCube( CVector3D( Randf( 15.0, 250.0f ), Randf( 50.0, 100.0f ), Randf( 150.0, 250.0f ) ) );
	}
	m_physicsActor = 0;
}

/*----------------------------------------------------------------------------------------------------------------------*/

CMesh::CMesh( const CVector3D &size )
{
	CreateCube( size );
	m_physicsActor = 0;
}

/*----------------------------------------------------------------------------------------------------------------------*/

CMesh::~CMesh( void )
{
	delete [] m_renderVertices;
	delete [] m_transformedVertices;
	delete [] m_renderNormals;
	delete [] m_vertexColor;
	delete [] m_renderIndices;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CMesh::Render( void )
{
	m_transformMatrix.Identity();
	if( m_physicsActor ) {
		PxTransform trans = static_cast<PxRigidDynamic *>( m_physicsActor )->getGlobalPose();
		CQuat q( trans.q.x, trans.q.y, trans.q.z, trans.q.w );
		CVector3D rot;
		q.GetEulers( &rot.x, &rot.y, &rot.z );
		m_transformMatrix.RotaZXY( rot.x, rot.y, rot.z );
		m_transformMatrix.SetTranslation( CVector3D( trans.p.x, trans.p.y, trans.p.z ) );
		for( unsigned int i=0; i<m_numRenderVertices; ++i ) {
			m_transformMatrix.Multiply( m_renderVertices[i], m_transformedVertices[i] );
		}
	}
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, m_transformedVertices );
	glNormalPointer( GL_FLOAT, 0, m_renderNormals );
	glColorPointer( 4, GL_FLOAT, 0, m_vertexColor );
	glEnableClientState( GL_ELEMENT_ARRAY_BUFFER );
	glDrawElements( GL_TRIANGLES, m_numRenderIndices, GL_UNSIGNED_INT, m_renderIndices );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_ELEMENT_ARRAY_BUFFER );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CMesh::CreateCube( const CVector3D &size )
{
	CVector3D center( 0.0f, 0.0f, 0.0f );
	m_numRenderVertices = 24;
	m_numRenderIndices = 36;
	m_renderVertices = new SIMPLE_VERTEX[m_numRenderVertices];
	m_renderNormals = new SIMPLE_VERTEX[m_numRenderVertices];
	m_transformedVertices = new SIMPLE_VERTEX[m_numRenderVertices];
	m_vertexColor = new VERTEX_ATTRIB4_FLOAT[m_numRenderVertices];
	m_renderIndices = new unsigned int[m_numRenderIndices];
	unsigned int processedVertices = 0;
	unsigned int processedIndices = 0;
	VERTEX_ATTRIB4_FLOAT defaultColor;
	defaultColor.x = Randf( 0.0f, 0.5f );
	defaultColor.y = Randf( 0.0f, 0.5f );
	defaultColor.z = Randf( 0.0f, 0.5f );
	defaultColor.w = Randf( 0.3f, 0.3f );

	for( unsigned int i=0; i<m_numRenderVertices; ++i ) {
		m_vertexColor[i] = defaultColor;
	}

	// Cubo superior
	float sizeX = size.x;
	float sizeY = size.y;
	float sizeZ = size.z;
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX, 1.0f * sizeY,-1.0f * sizeZ).ToSimpleVertex();          // Top Right Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX, 1.0f * sizeY,-1.0f * sizeZ).ToSimpleVertex();          // Top Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ).ToSimpleVertex();         // Bottom Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ).ToSimpleVertex();          // Bottom Right Of The Quad (Top) 

	m_renderIndices[processedIndices++] = processedVertices - 4;
	m_renderIndices[processedIndices++] = processedVertices - 3;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 1;
	m_renderIndices[processedIndices++] = processedVertices - 4;

	CVector3D aux, edge1, edge2;
	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-2]], m_renderVertices[m_renderIndices[processedIndices-1]], edge1 );
	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-1]], m_renderVertices[m_renderIndices[processedIndices-2]], edge2 );
	aux.Cross( edge2, edge1 );
	for( unsigned int i=processedVertices-4; i<processedVertices; ++i ) {
		m_renderNormals[i] = aux.ToSimpleVertex(); 
	}

	// Cubo inferior
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX,-1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Top Right Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX,-1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Top Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX,-1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();         // Bottom Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX,-1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();          // Bottom Right Of The Quad (Top) 

	m_renderIndices[processedIndices++] = processedVertices - 4;
	m_renderIndices[processedIndices++] = processedVertices - 3;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 1;
	m_renderIndices[processedIndices++] = processedVertices - 4;

	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-2]], m_renderVertices[m_renderIndices[processedIndices-1]], edge1 );
	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-1]], m_renderVertices[m_renderIndices[processedIndices-2]], edge2 );
	aux.Cross( edge2, edge1 );
	for( unsigned int i=processedVertices-4; i<processedVertices; ++i ) {
		m_renderNormals[i] = aux.ToSimpleVertex(); 
	}

	// Cubo frontal
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Top Right Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Top Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX,-1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();         // Bottom Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX,-1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Bottom Right Of The Quad (Top) 

	m_renderIndices[processedIndices++] = processedVertices - 4;
	m_renderIndices[processedIndices++] = processedVertices - 3;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 1;
	m_renderIndices[processedIndices++] = processedVertices - 4;

	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-2]], m_renderVertices[m_renderIndices[processedIndices-1]], edge1 );
	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-1]], m_renderVertices[m_renderIndices[processedIndices-2]], edge2 );
	aux.Cross( edge2, edge1 );
	for( unsigned int i=processedVertices-4; i<processedVertices; ++i ) {
		m_renderNormals[i] = aux.ToSimpleVertex(); 
	}

	// Cubo trasero
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX,-1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();          // Top Right Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX,-1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();          // Top Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX, 1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();         // Bottom Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX, 1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();          // Bottom Right Of The Quad (Top) 
	
	m_renderIndices[processedIndices++] = processedVertices - 4;
	m_renderIndices[processedIndices++] = processedVertices - 3;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 1;
	m_renderIndices[processedIndices++] = processedVertices - 4;

	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-2]], m_renderVertices[m_renderIndices[processedIndices-1]], edge1 );
	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-1]], m_renderVertices[m_renderIndices[processedIndices-2]], edge2 );
	aux.Cross( edge2, edge1 );
	for( unsigned int i=processedVertices-4; i<processedVertices; ++i ) {
		m_renderNormals[i] = aux.ToSimpleVertex(); 
	}

	// Cubo izquierdo
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Top Right Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX, 1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();          // Top Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX,-1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();         // Bottom Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( -1.0f * sizeX,-1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Bottom Right Of The Quad (Top) 

	m_renderIndices[processedIndices++] = processedVertices - 4;
	m_renderIndices[processedIndices++] = processedVertices - 3;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 1;
	m_renderIndices[processedIndices++] = processedVertices - 4;

	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-2]], m_renderVertices[m_renderIndices[processedIndices-1]], edge1 );
	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-1]], m_renderVertices[m_renderIndices[processedIndices-2]], edge2 );
	aux.Cross( edge2, edge1 );
	for( unsigned int i=processedVertices-4; i<processedVertices; ++i ) {
		m_renderNormals[i] = aux.ToSimpleVertex(); 
	}

	// Cubo derecho
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX, 1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();          // Top Right Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();          // Top Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX,-1.0f * sizeY, 1.0f * sizeZ ).ToSimpleVertex();         // Bottom Left Of The Quad (Top) 
	m_renderVertices[processedVertices++] = CVector3D( 1.0f * sizeX,-1.0f * sizeY,-1.0f * sizeZ ).ToSimpleVertex();          // Bottom Right Of The Quad (Top) 

	m_renderIndices[processedIndices++] = processedVertices - 4;
	m_renderIndices[processedIndices++] = processedVertices - 3;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 2;
	m_renderIndices[processedIndices++] = processedVertices - 1;
	m_renderIndices[processedIndices++] = processedVertices - 4;

	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-2]], m_renderVertices[m_renderIndices[processedIndices-1]], edge1 );
	VertexDifference( m_renderVertices[m_renderIndices[processedIndices-1]], m_renderVertices[m_renderIndices[processedIndices-2]], edge2 );
	aux.Cross( edge2, edge1 );
	for( unsigned int i=processedVertices-4; i<processedVertices; ++i ) {
		m_renderNormals[i] = aux.ToSimpleVertex(); 
	}

	for( unsigned int i=0; i<m_numRenderVertices; ++i ) {
		m_transformedVertices[i] = m_renderVertices[i];
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/
