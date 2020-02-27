
// Includes
#include "Level.h"
#include "stdio.h"
#include "windows.h"
#include "GL/glew.h"
#include "Math/Vertex.h"

// Estructura auxiliar para guardar los meshes en la carga
struct MeshInfo
{
	SIMPLE_VERTEX *vertices;
	SIMPLE_VERTEX *normals;
	unsigned int *indices;
	unsigned int numVertices;
	unsigned int numIndices;
};

/*----------------------------------------------------------------------------------------------------------------------*/

SIMPLE_VERTEX GenerateVertex( float x, float y, float z )
{
	SIMPLE_VERTEX sv;
	sv.x = x;
	sv.y = y;
	sv.z = z;
	return sv;
}

/*----------------------------------------------------------------------------------------------------------------------*/

CLevel::CLevel( void )
{
	// Cargamos el mapita
	m_numRenderIndices = 0;
	m_numRenderVertices = 0;
	FILE *fileHandler = fopen( "map.cds", "rb" );
	unsigned int numMeshes = 0;
	if( fileHandler ){
		fread( &numMeshes, 1, sizeof( unsigned int ), fileHandler );
		MeshInfo *meshInfo = new MeshInfo[numMeshes];
		for( unsigned int i=0; i<numMeshes; ++i ){
			fread( &meshInfo[i].numVertices, 1, sizeof( int ), fileHandler );
			meshInfo[i].vertices = new SIMPLE_VERTEX[meshInfo[i].numVertices];
			fread( meshInfo[i].vertices, meshInfo[i].numVertices, 3 * sizeof( float ), fileHandler );
			meshInfo[i].normals = new SIMPLE_VERTEX[meshInfo[i].numVertices];
			fread( meshInfo[i].normals, meshInfo[i].numVertices, 3 * sizeof( float ), fileHandler );
			fread( &meshInfo[i].numIndices, 1, sizeof( unsigned int ), fileHandler );
			meshInfo[i].indices = new unsigned int[meshInfo[i].numIndices];
			fread( meshInfo[i].indices, meshInfo[i].numIndices, sizeof( unsigned int ), fileHandler );
			m_numRenderVertices += meshInfo[i].numVertices;
			m_numRenderIndices += meshInfo[i].numIndices;
		}

		// Para el suelo...
		m_numRenderVertices += 4;
		m_numRenderIndices += 6;

		// Creamos el temita
		m_renderVertices = new SIMPLE_VERTEX[m_numRenderVertices];
		m_renderNormals = new SIMPLE_VERTEX[m_numRenderVertices];
		m_vertexColor = new VERTEX_ATTRIB4_FLOAT[m_numRenderVertices];
		m_renderIndices = new unsigned int[m_numRenderIndices];
		unsigned int processedVertices = 0;
		unsigned int processedIndices = 0;
		for( unsigned int m=0; m<numMeshes; ++m ) {
			for( unsigned int i=0; i<meshInfo[m].numIndices; ++i ) {
				m_renderIndices[processedIndices++] = meshInfo[m].indices[i] + processedVertices;
			}
			for( unsigned int v=0; v<meshInfo[m].numVertices; ++v ) {
				m_vertexColor[processedVertices].x = 0.0f;
				m_vertexColor[processedVertices].y = 0.0f;
				m_vertexColor[processedVertices].z = 0.3f;
				m_vertexColor[processedVertices].w = 0.5f;
				m_renderNormals[processedVertices] = meshInfo[m].normals[v];
				m_renderVertices[processedVertices++] = meshInfo[m].vertices[v];
				Multiply( &m_renderVertices[processedVertices-1], 1.5f );
			}
		}

		// El suelo...
		m_vertexColor[processedVertices].x = 0.0f;
		m_vertexColor[processedVertices].y = 0.3f;
		m_vertexColor[processedVertices].z = 0.0f;
		m_vertexColor[processedVertices].w = 0.5f;
		m_renderNormals[processedVertices] = GenerateVertex( 0.0f, 1.0f, 0.0f );
		m_renderVertices[processedVertices++] = GenerateVertex( -10000.0f, 0.0f, 10000.0f );

		m_vertexColor[processedVertices].x = 0.0f;
		m_vertexColor[processedVertices].y = 0.3f;
		m_vertexColor[processedVertices].z = 0.0f;
		m_vertexColor[processedVertices].w = 0.5f;
		m_renderNormals[processedVertices] = GenerateVertex( 0.0f, 1.0f, 0.0f );
		m_renderVertices[processedVertices++] = GenerateVertex( 10000.0f, 0.0f, 10000.0f );

		m_vertexColor[processedVertices].x = 0.0f;
		m_vertexColor[processedVertices].y = 0.3f;
		m_vertexColor[processedVertices].z = 0.0f;
		m_vertexColor[processedVertices].w = 0.5f;
		m_renderNormals[processedVertices] = GenerateVertex( 0.0f, 1.0f, 0.0f );
		m_renderVertices[processedVertices++] = GenerateVertex( 10000.0f, 0.0f, -10000.0f );

		m_vertexColor[processedVertices].x = 0.0f;
		m_vertexColor[processedVertices].y = 0.3f;
		m_vertexColor[processedVertices].z = 0.0f;
		m_vertexColor[processedVertices].w = 0.5f;
		m_renderNormals[processedVertices] = GenerateVertex( 0.0f, 1.0f, 0.0f );
		m_renderVertices[processedVertices++] = GenerateVertex( -10000.0f, 0.0f, -10000.0f );

		m_renderIndices[processedIndices++] = m_numRenderVertices - 2;
		m_renderIndices[processedIndices++] = m_numRenderVertices - 3;
		m_renderIndices[processedIndices++] = m_numRenderVertices - 4;
		m_renderIndices[processedIndices++] = m_numRenderVertices - 4;
		m_renderIndices[processedIndices++] = m_numRenderVertices - 1;
		m_renderIndices[processedIndices++] = m_numRenderVertices - 2;

		// Release de la info auxiliar
		for( unsigned int i=0; i<numMeshes; ++i ) {
			delete [] meshInfo[i].vertices;
			delete [] meshInfo[i].normals;
			delete [] meshInfo[i].indices;
		}
		delete [] meshInfo;

		// Fin de lectura...
		fclose( fileHandler );

		// invertimos los triangulos
		unsigned int aux;
		for( unsigned int i=0; i<m_numRenderIndices; i+=3 ) {
			aux = m_renderIndices[i];
			m_renderIndices[i] = m_renderIndices[i+2];
			m_renderIndices[i+2] = aux;
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

CLevel::~CLevel( void )
{
	delete [] m_renderVertices;
	delete [] m_renderNormals;
	delete [] m_vertexColor;
	delete [] m_renderIndices;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CLevel::Render( void )
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, m_renderVertices );
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
