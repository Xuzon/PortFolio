
// Includes
#include "Ball.h"
#include "../GL/glew.h"
#include "math.h"
#include "DemoApp.h"

#define Frac_Circ 15  // 25 fracciones de circulo
#define PI 3.1415926535897932

// Globales
CPhysics *g_PhysicsPtr;
extern CDemoApp *g_demoPtr;

/*----------------------------------------------------------------------------------------------------------------------*/

CBall::CBall( int resX, int resY, const CVector3D &pos )
{
	m_resX = resX;
	m_resY = resY;
	m_pos = pos;
	m_radius = 0.025f;	 

	// Lo añadimos al gestor de fisica
	m_physicsActor = g_PhysicsPtr->AddDynamicActor( m_pos, m_radius );
	m_physicsActor->report = this;
}

/*----------------------------------------------------------------------------------------------------------------------*/

CBall::~CBall( void )
{
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CBall::Render( void )
{
	m_pos = m_physicsActor->pos;
	//g_demoPtr->DrawText( 1, m_pos.x * m_resX - 15, m_pos.y * m_resY + 4, 1.0f, 1.0f, 1.0f, "%0.1f", m_physicsActor->mat.restitution );
	glLineWidth( 3.0f );
	glBegin( GL_POLYGON );
		for ( int i = 0; i < Frac_Circ; i++ ) {  // +1 para cerrar
			!m_physicsActor->sleeping ? glColor4f(color.x,color.y,color.z,1) : glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
			glVertex2f( ( m_pos.x + m_radius * cosf( 2.0f * static_cast<float>( PI ) * i / Frac_Circ ) ) * m_resX,
				( m_pos.y + m_radius * sinf( 2.0f * static_cast<float>( PI ) * i / Frac_Circ ) ) * m_resY );
		}
	glEnd();
}

void CBall::OnContact()
{
	color = CVector3D(0, 1, 1);
}

/*----------------------------------------------------------------------------------------------------------------------*/
