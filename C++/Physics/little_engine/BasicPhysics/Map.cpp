
// Includes
#include "Map.h"
#include "../GL/glew.h"
#include "Physics.h"

// Globales
extern CPhysics *g_PhysicsPtr;

/*----------------------------------------------------------------------------------------------------------------------*/

CMap::CMap( int resX, int resY )
{
	// Guardamos resolucion
	m_resX = resX;
	m_resY = resY;

	// En coordenadas normalizadas
	m_numSegments = 0;
	m_segments[m_numSegments].p[0] = CVector3D( 0.01f, 0.01f, 0.0f ); 
	m_segments[m_numSegments].p[1] = CVector3D( 0.01f, 0.99f, 0.0f );
	++m_numSegments;
	m_segments[m_numSegments].p[0] = CVector3D( 0.01f, 0.99f, 0.0f ); 
	m_segments[m_numSegments].p[1] = CVector3D( 0.99f, 0.99f, 0.0f );
	++m_numSegments;
	m_segments[m_numSegments].p[0] = CVector3D( 0.99f, 0.99f, 0.0f ); 
	m_segments[m_numSegments].p[1] = CVector3D( 0.99f, 0.01f, 0.0f );
	++m_numSegments;

	//m_segments[m_numSegments].p[0] = CVector3D( 0.3f, 0.3f, 0.0f ); 
	//m_segments[m_numSegments].p[1] = CVector3D( 0.7f, 0.9f, 0.0f );
	//++m_numSegments;

	// Lo añadimos al gestor de fisica
	g_PhysicsPtr->AddStaticActor( m_segments, m_numSegments );

}

/*----------------------------------------------------------------------------------------------------------------------*/

CMap::~CMap( void )
{
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CMap::Render( void )
{
	glLineWidth( 10.0f );
	for( unsigned int i=0; i<m_numSegments; ++i ) {
		sSegment &current = m_segments[i];
		glBegin( GL_LINES );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glVertex3f( current.p[0].x * m_resX, current.p[0].y * m_resY, current.p[0].z );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glVertex3f( current.p[1].x * m_resX, current.p[1].y * m_resY, current.p[1].z );
		glEnd();
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CMap::AddSegment( const CVector3D &p0, const CVector3D &p1 )
{
	m_segments[m_numSegments].p[0] = p0; 
	m_segments[m_numSegments].p[1] = p1;
	++m_numSegments;

	g_PhysicsPtr->AddStaticActor( m_segments + m_numSegments - 1, 1 );
}

/*----------------------------------------------------------------------------------------------------------------------*/
