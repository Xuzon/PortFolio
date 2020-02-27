
// Includes
#include "pxtask/PxCudaContextManager.h"
#include "PxSimulationEventCallback.h"
#include "PxShape.h"
#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"
#include "characterkinematic/PxCapsuleController.h"
#include "extensions/PxExtensionsAPI.h"
//#include "PsFile.h"
//#include "PsShare.h"
//#include "PxTask.h"
#include "PxDefaultSimulationFilterShader.h"
#include "PxFiltering.h"
//#include "PhysX/PxToolkit/include/PxTkStream.h"

// Forwards
class CLevel;
class CMesh;
class CVector3D;
class CCharDisplacer; // @TODO 6
class CTrigger;

using namespace physx;

// Definicion del gestor de fisicas
class CPhysicsWorld : public physx::PxSimulationEventCallback // @TODO 5
{
public:

	// Constructor/destructor
	CPhysicsWorld( void );
	~CPhysicsWorld( void );

	// updade
	void Update( float timeDiff );

	// Inserta el nivel en el mundo de colision
	CLevel *AddLevelToPhysicsWorld( void );

	// Tirada ed rayo
	bool RayTest( const CVector3D &orig, const CVector3D &dir, float distance, CVector3D &res );

	// Inserta un actor dinamico al mundo de colision
	CMesh *AddRigidBodyDynamic( const CVector3D &pos, const CVector3D &iniVel );

	// Crea un controlador de personajes 
	CCharDisplacer *CreateCharDisplacer( const CVector3D &pos ); // @TODO 6

	// Crea un trigger
	CMesh *CreateTrigger( const CVector3D &pos, const CVector3D &size ); // @TODO TRIGGER

	// Implements PxSimulationEventCallback
	virtual	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
	virtual void onSleep(physx::PxActor** actors, physx::PxU32 count){};
	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count){};
	virtual void onWake(physx::PxActor** actors, physx::PxU32 count){};

private:

	physx::PxPhysics *m_sdk;
	physx::PxCooking *m_cooking;
	physx::PxScene *m_scene;
	physx::PxMaterial *m_material;
	physx::PxDefaultCpuDispatcher *m_cpuDispatcher;
	physx::PxControllerManager *m_controllerManager;
	float m_slotTimer = 0.0f;
};