
// Include
#include "Trigger.h"
#include "DemoApp.h"

extern CDemoApp *g_demoPtr;
/*----------------------------------------------------------------------------------------------------------------------*/

CTrigger::CTrigger( const CVector3D &pos, const CVector3D &size ) : CMesh( size )
{
	for( unsigned int i=0; i<m_numRenderVertices; ++i ) {
		Add( m_transformedVertices[i], pos.ToSimpleVertex() );
	}
	for( unsigned int i=0; i<m_numRenderVertices; ++i ) {
		m_vertexColor[i].x = 1.0f;
		m_vertexColor[i].y = 1.0f;
		m_vertexColor[i].z = 0.0f;
		m_vertexColor[i].w = 0.2f;
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

CTrigger::~CTrigger( void )
{
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CTrigger::OnTrigger( bool enter )
{
	g_demoPtr->InvertGravity( 2.0f );
}

/*----------------------------------------------------------------------------------------------------------------------*/
