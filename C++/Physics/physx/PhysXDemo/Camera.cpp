
// Includes
#include "windows.h"
#include "GL/gl.h"
#include "Camera.h"
#include "math.h"
#include "DemoApp.h"

#define _2PI 6.2892f

/*----------------------------------------------------------------------------------------------------------------------*/

int CCamera::LeftCicle = 0;
bool CCamera::LeftClicked = false;

/*----------------------------------------------------------------------------------------------------------------------*/

CCamera::CCamera( void )
{

}

/*----------------------------------------------------------------------------------------------------------------------*/

CCamera::~CCamera( void )
{

}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::Transform2Dto3D( float x, float y,  CVector3D &vcOrig,  CVector3D &vcDir )
{
	CMatrix4x4 *pView = NULL, *pProj = NULL;
  CMatrix4x4 mInvView;
  CVector3D vcS;
  unsigned long dwWidth, dwHeight;
  dwWidth  = m_width;
  dwHeight = m_height;

	pView = &m_view;
	pProj = &m_proj;
  

  // Cambiamos el rango del viewport a [-1,1]
  vcS.x =  ( ( ( x * 2.0f ) / (dwWidth) ) -1.0f ) / pProj->_11;
  vcS.y = -( ( ( y * 2.0f ) / (dwHeight) ) -1.0f ) / pProj->_22;
  vcS.z = 1.0f;

  // Invertimos la matriz de la vista
  mInvView.InverseOf( *( ( CMatrix4x4 * ) pView ) );

  vcDir.x = ( vcS.x * mInvView._11 )
        + ( vcS.y * mInvView._21 )
        + (vcS.z * mInvView._31);
  vcDir.y = ( vcS.x * mInvView._12 )
        + ( vcS.y * mInvView._22 )
        + ( vcS.z * mInvView._32 );
  vcDir.z = ( vcS.x * mInvView._13 )
        + ( vcS.y * mInvView._23 )
        + ( vcS.z * mInvView._33 );
  vcDir.Normalize();
  vcOrig.x = mInvView._41;
  vcOrig.y = mInvView._42;
  vcOrig.z = mInvView._43;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::Init( unsigned int width, unsigned int height )
{
	m_width = width;
	m_height = height;
	m_near = 1.0f;             
	m_far = 200000.0f;
	m_view.Identity();
	m_proj.Identity();
	m_world.Identity();
	m_viewProj.Identity();
	m_modelView.Identity();
	m_worldViewProj.Identity();
	m_rotX = m_rotY = m_rotZ = 0.0f;

	// Calculamos el aspect-ratio
	float myAspect = static_cast<float>( height ) / static_cast<float>( width );

	// Calculamos la matriz de proyección en perspectiva
	CalcProjMatrix( 0.8f, myAspect );
	CalcViewProjMatrix();

	// Hacemos efectivo el viewport en la GPU
	glViewport( 0, 0, m_width, m_height );
	m_speed = 10000.0f;
	m_pos = CVector3D( 0.0f, 10.0f, 0.0f );
	m_right = CVector3D( 1, 0, 0 );
	m_up = CVector3D( 0, 1, 0 );
	m_dir = CVector3D( 0.5, -0.5, 0.5 );
	m_dir.Normalize();
	m_pos = m_dir * -3000.0f;
	SetView3D( m_right, m_up, m_dir, m_pos );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::Update( EVENT &event, float timeDiff )
{
	if( event.mouse.leftButton.down ){
		LeftCicle += 1;
		LeftClicked = true;
	}
	// Modo giro o no
	if( event.mouse.rightButton.down ){
		m_rightMousePresed = true;
		ShowCursor( false );
	}
	if( event.mouse.rightButton.up ){
		m_rightMousePresed = false;
		ShowCursor( true );
	}

	// Velocidad de movimiento
	m_speed += event.mouse.wheel;

	// Flecha derecha
	if( event.key['d'].down || event.key['D'].down ){
		StepRight( timeDiff * m_speed );
	}

	// Flecha izquierda
	if( event.key['a'].down || event.key['A'].down ){
		StepLeft( timeDiff * m_speed );
	}

	// Flecha Arriba
	if( event.key['w'].down || event.key['W'].down ){
		MoveForward( timeDiff * m_speed );
	}

	// Flecha abajo
	if( event.key['s'].down || event.key['S'].down ){
		MoveBackward( timeDiff * m_speed );
	}

	// Si ha habido movimiento de ratón
	if( event.mouse.deltax != 0.0f || event.mouse.deltay != 0.0f ){

		// Giramos la mirada
		if( m_rightMousePresed ){
			AddRot( static_cast<float>( event.mouse.deltay ) * 0.005f, static_cast<float>( event.mouse.deltax ) * 0.005f, 0 );
		}
	}

	if( m_rightMousePresed ){
		event.mouse.x = m_width / 2;
		event.mouse.y = m_height / 2;
		event.mouse.oldx = event.mouse.x;
		event.mouse.oldy = event.mouse.y;
		SetCursorPos( m_width / 2, m_height / 2 );
	}

	//// Actualizamos el vector de velocidades
	//m_speed = m_dir * m_thrust * elapsedTime;

	//// Actualizamos la posición
	//m_pos += m_speed;

	// Recalculamos los ejes
	CMatrix4x4 rotMat;
	m_right = CVector3D( 1.0f, 0.0f, 0.0f );
	m_dir = CVector3D( 0.0f, 0.0f, 1.0f );
	m_up = CVector3D( 0.0f, 1.0f, 0.0f );
	rotMat.Identity();
	rotMat.RotaZXY( m_rotX, m_rotY, m_rotZ );
	m_dir = m_dir * rotMat;
	m_right = m_right * rotMat;
	m_up = m_up * rotMat;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::AddRot( float x, float y, float z )
{
	m_rotX += x;
	m_rotY += y;
	m_rotZ += z;

	while( m_rotX > _2PI )
		m_rotX -= _2PI;
	while( m_rotY > _2PI )
		m_rotY -= _2PI;
	while( m_rotZ > _2PI )
		m_rotZ -= _2PI;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::SetPos( const CVector3D &p )
{
	m_pos = p;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::SetDir( const CVector3D &vc )
{
	m_dir = vc;
	m_dir.Normalize();
	CVector3D res = vc;
	m_rotZ = 0;
	m_rotY = (float)atan2(  vc.x, vc.z );
	m_rotX = (float)atan2( -vc.y, sqrt( vc.x*vc.x + vc.z*vc.z ) );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::StepLeft( float dist )
{
	m_pos -= ( m_right * dist );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::StepRight( float dist )
{
	m_pos += ( m_right * dist );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::StepUp( float dist )
{
	m_pos += ( m_up * dist );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::StepDown( float dist )
{
	m_pos -= ( m_up * dist );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::MoveForward( float dist )
{
	m_pos += ( m_dir * dist );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::MoveBackward( float dist )
{
	m_pos -= ( m_dir * dist );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::ApplyToOGL( void )
{
	glViewport( 0, 0, m_width, m_height );
	SetView3D( m_right, m_up, m_dir, m_pos );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( m_proj.v );  
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::CalcViewProjMatrix( void )
{
	m_viewProj = m_view * m_proj;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::CalcWorldViewProjMatrix( void )
{
	m_worldViewProj = ( m_world * m_view ) * m_proj;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::CalcModelViewMatrix( void )
{
	m_modelView = m_world * m_view;  
}

/*----------------------------------------------------------------------------------------------------------------------*/

bool CCamera::CalcProjMatrix( float fov, float aspect )
{
	if( fabs( m_far - m_near ) < 0.01f )
		return false;
	float sinFOV2 = sinf( fov / 2 );
	if( fabs( sinFOV2 ) < 0.01f )
		return false;
	float cosFOV2 = cosf( fov / 2 );
	float w = aspect * ( cosFOV2 / sinFOV2 );
	float h =   1.0f  * ( cosFOV2 / sinFOV2 );
	float Q = m_far / ( m_far - m_near );
	memset( &m_proj, 0, sizeof( CMatrix4x4 ) );
	m_proj._11 = w;
	m_proj._22 = h;
	m_proj._33 = Q;
	m_proj._34 = 1.0f;
	m_proj._43 = -Q * m_near;
	return true;
	//m_proj.Identity();
	//m_proj._11 =  2.0f / m_width;
	//m_proj._22 =  2.0f / m_height;
	//m_proj._33 =  1.0f / ( m_far - m_near );
	//m_proj._43 =  m_near /( m_near - m_far );
	//m_proj._44 =  1.0f;
	//return true;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCamera::SetView3D( const CVector3D &right , const CVector3D &up, const CVector3D &dir, const CVector3D &pos )
{
	// Asignamos los valores a la matriz de la vista 3D
	m_view._14 = m_view._21 = m_view._34 = 0.0f;
	m_view._44 = 1.0f;

	m_view._11 = right.x;
	m_view._21 = right.y;
	m_view._31 = right.z;
	m_view._41 = -( right * pos );

	m_view._12 = up.x;
	m_view._22 = up.y;
	m_view._32 = up.z;
	m_view._42 = -( up * pos );

	m_view._13 = dir.x;
	m_view._23 = dir.y;
	m_view._33 = dir.z;
	m_view._43 = -( dir * pos );

	// Recalculamos la matriz Model-View
	CalcModelViewMatrix();
	glMatrixMode( GL_MODELVIEW ); 
	glLoadMatrixf( m_modelView.v );

	// Recalculamos las demás matrices dependientes
	CalcViewProjMatrix();
	CalcWorldViewProjMatrix();
}

/*----------------------------------------------------------------------------------------------------------------------*/
