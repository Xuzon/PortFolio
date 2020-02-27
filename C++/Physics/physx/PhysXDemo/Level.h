#ifndef _LEVEL_H_ 
#define _LEVEL_H_

// Includes

// Forwards
struct SIMPLE_VERTEX;
struct VERTEX_ATTRIB4_FLOAT;
class CPhysicsWorld;

// Definicion de la clase
class CLevel
{
public:
	CLevel( void );
	~CLevel( void );
	void Render( void );

private:

	// Para agilizar la demo...
	friend class CPhysicsWorld;

	// info de geometria
	SIMPLE_VERTEX *m_renderVertices;
	SIMPLE_VERTEX *m_renderNormals;
	VERTEX_ATTRIB4_FLOAT *m_vertexColor;
	unsigned int *m_renderIndices;
	unsigned int m_numRenderVertices;
	unsigned int m_numRenderIndices;
};

#endif