
// includes
#include "DebugUtils.h"
#include "windows.h"
#include "GL/glew.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector3D.h"
#include "math.h"

// defines
#define M_PI 3.1416f

/*----------------------------------------------------------------------------------------------------------------------*/

void DebugRenderCapsule( CMatrix4x4 &matrix, float l, float r, const CVector3D &color )
{
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glMultMatrixf( matrix.v );
	static const int capped_cylinder_quality = 3;

	//RenderWorldMatrix(MTr, false);

	int i,j;
	float tmp,nx,ny,nz,start_nx,start_ny,a,ca,sa;
	const int n = capped_cylinder_quality*4;

	l *= 0.5;
	a = float(M_PI*2.0)/float(n);
	sa = (float) sin(a);
	ca = (float) cos(a);

	// draw cylinder body
	ny=1; nz=0;		  // normal vector = (0,ny,nz)
	for (i=0; i<=n; i++)
	{
		DebugRenderLine3D(CVector3D(ny*r, nz*r, l), CVector3D(ny*r, nz*r, -l), color);

		// rotate ny,nz
		tmp = ca*ny - sa*nz;
		nz = sa*ny + ca*nz;
		ny = tmp;
	}

	// draw first cylinder cap
	start_nx = 0;
	start_ny = 1;
	for (j=0; j<(n/4); j++)
	{
		// get start_n2 = rotated start_n
		float start_nx2 =  ca*start_nx + sa*start_ny;
		float start_ny2 = -sa*start_nx + ca*start_ny;
		// get n=start_n and n2=start_n2
		nx = start_nx; ny = start_ny; nz = 0;
		float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
		for (i=0; i<=n; i++)
		{
			DebugRenderLine3D(CVector3D(ny2*r,nz2*r,l+nx2*r), CVector3D(ny*r,nz*r,l+nx*r), color);

			// rotate n,n2
			tmp = ca*ny - sa*nz;
			nz = sa*ny + ca*nz;
			ny = tmp;
			tmp = ca*ny2- sa*nz2;
			nz2 = sa*ny2 + ca*nz2;
			ny2 = tmp;
		}
		start_nx = start_nx2;
		start_ny = start_ny2;
	}

	// draw second cylinder cap
	start_nx = 0;
	start_ny = 1;
	for (j=0; j<(n/4); j++)
	{
		// get start_n2 = rotated start_n
		float start_nx2 = ca*start_nx - sa*start_ny;
		float start_ny2 = sa*start_nx + ca*start_ny;
		// get n=start_n and n2=start_n2
		nx = start_nx; ny = start_ny; nz = 0;
		float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
		for (i=0; i<=n; i++)
		{
			DebugRenderLine3D(CVector3D(ny*r,nz*r,-l+nx*r), CVector3D(ny2*r,nz2*r,-l+nx2*r), color);

			// rotate n,n2
			tmp = ca*ny - sa*nz;
			nz = sa*ny + ca*nz;
			ny = tmp;
			tmp = ca*ny2- sa*nz2; 
			nz2 = sa*ny2 + ca*nz2;
			ny2 = tmp;
		}
		start_nx = start_nx2;
		start_ny = start_ny2;
	}
	glPopMatrix();
}

/*----------------------------------------------------------------------------------------------------------------------*/

void DebugRenderLine3D( const CVector3D &p1, const CVector3D &p2, const CVector3D &color )
{
	glBegin( GL_LINES );
	glColor4f( color.x, color.y, color.z, color.w );
	glVertex3f( p1.x, p1.y, p1.z );
	glColor4f( color.x, color.y, color.z, color.w );
	glVertex3f( p2.x, p2.y, p2.z );
	glEnd();
}

/*----------------------------------------------------------------------------------------------------------------------*/

void DebugRenderTriangle( const CVector3D &p1, const CVector3D &p2, const CVector3D &p3, bool edges, const CVector3D &color )
{
	glBegin( GL_TRIANGLES );
	glColor4f( color.x, color.y, color.z, color.w );
	glVertex3f( p1.x, p1.y, p1.z );
	glColor4f( color.x, color.y, color.z, color.w );
	glVertex3f( p2.x, p2.y, p2.z );
	glColor4f( color.x, color.y, color.z, color.w );
	glVertex3f( p3.x, p3.y, p3.z );
	glEnd();
	if( edges ) {
		CVector3D newColor = color;
		newColor.w = 1.0f;
		if( color.w == 1.0f ) {
			newColor.x = 1.0f;
		}
		DebugRenderLine3D( p1, p2, newColor );
		DebugRenderLine3D( p1, p3, newColor );
		DebugRenderLine3D( p2, p3, newColor );
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void DebugRenderSphere( const CVector3D &pos, float radius, const CVector3D &color, int divV, int divH )
{
#define MAX_DIVY_ESFERA 32
	CVector3D columna[ MAX_DIVY_ESFERA ];
	int nEcu = 10;
	int nMer = 10;
	int i,j;
	if ( radius <= 0 )
		return;

	if ( divH == 0 )
		divH = divV;
	nMer = divV;
	nEcu = divH;

	// calcula la primera hilera
	for ( i=0; i<=nEcu; i++ )
	{
		float angZ = i * (float)(3.1416f/nEcu);
		columna[ i ].x = pos.x + (float)sin( angZ ) * radius;
		columna[ i ].y = pos.y + (float)cos( angZ ) * radius;
		columna[ i ].z = pos.z;
	}

	for ( j=1; j<=nMer; j++ )
	{
		float angY = j * (float)(2*3.1416f/nMer);
		float senY, cosY;

		//sincos( angY, &senY, &cosY );
		senY = sin( angY );
		cosY = cos( angY );

		for ( i=1; i<nEcu; i++ )
		{
			float angZ = i * (float)(3.1416f/nEcu);
			float senZ, cosZ;
			SIMPLE_VERTEX V;

			//sincos( angZ, &senZ, &cosZ );
			senZ = sin( angZ );
			cosZ = cos( angZ );

			V.x = pos.x + cosY * senZ * radius;
			V.y = pos.y + cosZ * radius;
			V.z = pos.z + -senY * senZ * radius;

			//DrawLine3D( P, V, C );

			//RenderLines( columna[ i-1 ], V, C );
			DebugRenderLine3D( columna[ i-1 ], V, color );
			DebugRenderLine3D( columna[ i ], V, color );
			columna[ i ] = V;
		}
		DebugRenderLine3D( columna[i-1], columna[i], color );
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

IDebugRenderable::IDebugRenderable( void )
{
	m_mustDel = false;
	m_color = CVector3D( 0.0f, 0.0f, 0.0f );
	m_timeLeft = 0.0f;
}

/*----------------------------------------------------------------------------------------------------------------------*/

IDebugRenderable::~IDebugRenderable( void )
{
}

/*----------------------------------------------------------------------------------------------------------------------*/

void IDebugRenderable::Update( float timeDiff )
{
	m_timeLeft -= timeDiff;
	if( m_timeLeft < 0.0f ) {
		m_mustDel = true;
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugSphere::CDebugSphere( void )
: IDebugRenderable()
{
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugSphere::~CDebugSphere( void )
{

}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDebugSphere::Render( void )
{
	DebugRenderSphere( m_pos, m_radius, m_color, 10, 10 );
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugLine::CDebugLine( void )
: IDebugRenderable()
{

}

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugLine::~CDebugLine( void )
{

}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDebugLine::Render( void )
{
	DebugRenderLine3D( m_points[0], m_points[1], m_color );
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugUtilManager *CDebugUtilManager::m_instance = NULL;

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugUtilManager::CDebugUtilManager( void )
{
	ZeroMemory( m_primitives, sizeof( IDebugRenderable * ) * MAX_DEBUG_PRIMITIVES );
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugUtilManager::~CDebugUtilManager( void )
{
	for( unsigned int i=0; i<MAX_DEBUG_PRIMITIVES; ++i ) {
		if( m_primitives[i] ) {
			delete m_primitives[i];
			m_primitives[i] = NULL;
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDebugUtilManager *CDebugUtilManager::GetInstance( void )
{ 
	if( !m_instance ) {
		m_instance = new CDebugUtilManager();
	}
	return m_instance;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDebugUtilManager::Destroy( void )
{
	if( m_instance ) {
		delete m_instance;
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDebugUtilManager::Update( float timeDiff )
{
	for( unsigned int i=0; i<MAX_DEBUG_PRIMITIVES; ++i ) {
		if( m_primitives[i] ) {
			m_primitives[i]->Update( timeDiff );
			if( m_primitives[i]->m_mustDel ) {
				delete m_primitives[i];
				m_primitives[i] = NULL;
			}
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDebugUtilManager::Render( void )
{
	for( unsigned int i=0; i<MAX_DEBUG_PRIMITIVES; ++i ) {
		if( m_primitives[i] ) {
			m_primitives[i]->Render();
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/
void CDebugUtilManager::CreateSphere( const CVector3D &pos, float radius, const CVector3D &color, float time )
{
	for( unsigned int i=0; i<MAX_DEBUG_PRIMITIVES; ++i ) {
		if( !m_primitives[i] ) {
			CDebugSphere *theSphere = new CDebugSphere();
			theSphere->m_pos = pos;
			theSphere->m_radius = radius;
			theSphere->m_color = color;
			theSphere->m_timeLeft = time;
			m_primitives[i] = theSphere;
			return;
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDebugUtilManager::CreateLine( const CVector3D &p0, const CVector3D &p1, const CVector3D &color, float time )
{
	for( unsigned int i=0; i<MAX_DEBUG_PRIMITIVES; ++i ) {
		if( !m_primitives[i] ) {
			CDebugLine *theLine = new CDebugLine();
			theLine->m_points[0] = p0;
			theLine->m_points[1] = p1;
			theLine->m_color = color;
			theLine->m_timeLeft = time;
			m_primitives[i] = theLine;
			return;
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

