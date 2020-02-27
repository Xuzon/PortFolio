// @TODO 6
#ifndef _CHAR_DISPLACER_H_ 
#define _CHAR_DISPLACER_H_ 

// Includes
#include "Math/Vector3D.h"
#include "characterkinematic/PxCapsuleController.h"
#include "characterkinematic/PxController.h"


// Forwards
struct EVENT;

namespace physx
{
	class PxCapsuleController;
};

// Definicion de la clase
class CCharDisplacer : public physx::PxUserControllerHitReport
{
public:
	CCharDisplacer( void );
	~CCharDisplacer( void );
	void InitPhysXCtrl( physx::PxCapsuleController *ctrl );
	void Update( float timeDiff, EVENT &event );
	void Render( void );
	void Move( CVector3D &disp );
	CVector3D GetPos( void ) const;
	inline void InvertGravity( float time ) { m_crazyGravityTimeLeft = time; }

	void onControllerHit( const physx::PxControllersHit &hit ){};
	void onShapeHit( const physx::PxControllerShapeHit &  hit );  
	void onObstacleHit(const physx::PxControllerObstacleHit& hit){};
private:

	// Controlador de personajes de phisX
	physx::PxCapsuleController *m_physXCharCtrl;

	physx::PxControllerFilters m_filters;
	// Direccion de mirada
	CVector3D m_dir;

	// modo camara?
	bool m_cameraMode;

	// gravedad invertida?
	float m_crazyGravityTimeLeft;

	// Estamos girando? 
	bool m_turningLeft, m_turningRight, m_forward, m_backward;
};

#endif
