// @TODO 6
// Includes
#include "CharDisplacer.h"
#include "DemoApp.h"
#include "characterkinematic/PxCapsuleController.h"
#include "Math/Matrix4x4.h"
#include "windows.h"
#include "GL/glew.h"
#include "Camera.h"
#include "DebugUtils.h"
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"

using namespace physx;

#define M_PI 3.1416f

__inline float Randf( float min, float max )
{
	return (float)( ((rand()&32767)*(1.0/32767.0))*(max-min) + min );
}

extern CCamera g_camera;

/*----------------------------------------------------------------------------------------------------------------------*/

CCharDisplacer::CCharDisplacer( void )
{
	m_physXCharCtrl = NULL;
	m_dir = CVector3D( 0.0f, 0.0f, 1.0f );
	m_turningLeft = m_turningRight = m_forward = m_backward = false;
	m_cameraMode = false;
	m_crazyGravityTimeLeft = 0.0f;
}  

/*----------------------------------------------------------------------------------------------------------------------*/

CCharDisplacer::~CCharDisplacer( void )
{

}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCharDisplacer::InitPhysXCtrl( physx::PxCapsuleController *ctrl )
{
	m_physXCharCtrl = ctrl;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCharDisplacer::Update( float timeDiff, EVENT &event )
{
	// Simulamos gravedad
	CVector3D despl;
	if( m_crazyGravityTimeLeft > 0.0f ) {
		m_crazyGravityTimeLeft -= timeDiff;
		despl.y = 2000.0f * timeDiff;
	}
	else {
		despl.y = -2000.0f * timeDiff;
	}
	if( m_cameraMode ) {
		m_dir = g_camera.GetDir();
		m_dir.y = 0;
		m_dir.Normalize();
	}

	float speed = 600.0f;
	float rotSpeed = 3.1416f;
	if( event.key['f'].down || event.key['F'].down ) {
		m_turningLeft = true;
	}
	if( event.key['h'].down || event.key['H'].down ) {
		m_turningRight = true;
	}
	if( event.key['f'].up || event.key['F'].up ) {
		m_turningLeft = false;
	}
	if( event.key['h'].up || event.key['H'].up ) {
		m_turningRight = false;
	}
	if( event.key['t'].down || event.key['T'].down ) {
		m_forward = true;
	}
	if( event.key['g'].down || event.key['G'].down ) {
		m_backward = true;
	}
	if( event.key['t'].up || event.key['T'].up ) {
		m_forward = false;
	}
	if( event.key['g'].up || event.key['G'].up ) {
		m_backward = false;
	}
	if( event.key['c'].up || event.key['C'].up ) {
		m_cameraMode = !m_cameraMode;
		PxExtendedVec3 vecX = m_physXCharCtrl->getPosition();
		CVector3D pos = CVector3D( static_cast<float>( vecX.x ), static_cast<float>( vecX.y ), static_cast<float>( vecX.z ) );
		g_camera.SetPos( pos );
		g_camera.SetDir( m_dir );
	}
	CMatrix4x4 rotMat;
	rotMat.Identity();
	if( m_turningLeft ) {
		if( m_cameraMode ) {
			CVector3D right;
			right.Cross( CVector3D( 0.0f, 1.0f, 0.0f ), m_dir );
			despl -= right * timeDiff * speed;
		}
		else {
			rotMat.RotaY( -rotSpeed * timeDiff );
			m_dir.RotateWith( rotMat );
			m_dir.Normalize();
		}
	}
	if( m_turningRight ) {
		if( m_cameraMode ) {
			CVector3D right;
			right.Cross( CVector3D( 0.0f, 1.0f, 0.0f ), m_dir );
			despl += right * timeDiff * speed;
		}
		else {
			rotMat.RotaY( rotSpeed * timeDiff );
			m_dir.RotateWith( rotMat );
			m_dir.Normalize();
		}
	}
	if( m_backward ) {
		despl -= m_dir * timeDiff * speed;
	}
	if( m_forward ) {
		despl += m_dir * timeDiff * speed;
	}
	Move( despl );

	if( m_cameraMode ) {
		PxExtendedVec3 vecX = m_physXCharCtrl->getPosition();
		CVector3D pos = CVector3D( static_cast<float>( vecX.x ), static_cast<float>( vecX.y ), static_cast<float>( vecX.z ) );
		g_camera.SetPos( pos + CVector3D( 0.0f, m_physXCharCtrl->getHeight() / 2.0f, 0.0f ) );
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCharDisplacer::Render( void )
{
	CMatrix4x4 matrix, rotMat;
	matrix.Identity();
	rotMat.Identity();
	PxExtendedVec3 vecX = m_physXCharCtrl->getPosition();
	CVector3D pos = CVector3D( static_cast<float>( vecX.x ), static_cast<float>( vecX.y ), static_cast<float>( vecX.z ) );
	matrix.RotaX( 90.0f * 3.1416f / 180.0f );
	CVector3D rotAngs;
	rotMat.RotaY( rotAngs.y );
	CMatrix4x4 matrixAux = matrix * rotMat;
	matrix = matrixAux;
	matrix.SetTranslation( pos );
	if( !m_cameraMode ) {
		DebugRenderCapsule( matrix, m_physXCharCtrl->getHeight(), m_physXCharCtrl->getRadius(), CVector3D( 1.0f, 0.0f, 0.0f ) );
	}
	CVector3D right;
	right.Cross( CVector3D( 0.0f, 1.0f, 0.0f ), m_dir );
	pos.y -= m_physXCharCtrl->getHeight() / 2.0f;
	CVector3D p3 = pos - right * m_physXCharCtrl->getRadius();
	CVector3D p2 = pos + right * m_physXCharCtrl->getRadius();
	CVector3D p1 = pos + m_dir * 4.0f * m_physXCharCtrl->getRadius();
	CVector3D color = CVector3D( 1.0f, 0.0f, 0.0f );
	color.w = 0.5f;
	DebugRenderTriangle( p1, p2, p3, true, color );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCharDisplacer::Move( CVector3D &disp )
{
	m_physXCharCtrl->move( PxVec3( disp.x, disp.y, disp.z ), 0.1f, 0.7f, m_filters );
}

/*----------------------------------------------------------------------------------------------------------------------*/

CVector3D CCharDisplacer::GetPos( void ) const
{ 
	PxExtendedVec3 vecX = m_physXCharCtrl->getPosition();
	return CVector3D( static_cast<float>( vecX.x ), static_cast<float>( vecX.y ), static_cast<float>( vecX.z ) );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CCharDisplacer::onShapeHit( const PxControllerShapeHit &  hit )
{
	PxRigidDynamic* actor = hit.shape->getActor()->isRigidDynamic();
	if( actor) {
		if( hit.dir.y==0.0f) {
			PxReal coeff = actor->getMass() * hit.length * 500.0f;
			PxVec3 dir = hit.dir;
			dir.y = Randf( 0.0f, 0.3f );
			dir.normalize();
			PxRigidBodyExt::addForceAtLocalPos( *actor, dir * coeff, PxVec3( 0, 0, 0 ), PxForceMode::eIMPULSE, true );
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

