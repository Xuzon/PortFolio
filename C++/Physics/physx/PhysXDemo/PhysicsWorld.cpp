
// Includes
#include "PhysicsWorld.h"
#include "windows.h"
#include "Level.h"
#include "Mesh.h"
#include "Camera.h"
#include "DebugUtils.h"

#include "CharDisplacer.h" // @TODO 6
#include "Trigger.h" // @TODO TRIGGER

// forwards
class CDemoApp;


#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3Cooking_x86.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_X86.lib")

PxFilterFlags ContactFilterShader(	
	PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// Comportamiento estandard para triggers
	if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		return PxFilterFlag::eDEFAULT;
	}

	// Notificar todas las colisiones, e incluir la información de los puntos de colision
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_CONTACT_POINTS;

	// CCD
	//pairFlags |= PxPairFlag::eSWEPT_INTEGRATION_LINEAR;
	return PxFilterFlag::eDEFAULT;
}

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

// defines
#define	SAFE_RELEASE(x)	if(x){ x->release(); x = NULL;	}

// Externs
extern CCamera g_camera;

/*----------------------------------------------------------------------------------------------------------------------*/

CPhysicsWorld::CPhysicsWorld( void )
{
	// @TODO 1: Init de PhysX / creación de escena.
	int nbThreads = 1;

	// Setup del motor de fisicas
	PxTolerancesScale tolerance = PxTolerancesScale(); // Escala en la que la simulación se define
	tolerance.length = 100.0f;
	tolerance.mass = 10000.0f;
	tolerance.speed = 98.0f;

	PxFoundation& foundation = *PxCreateFoundation( PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback );


	m_sdk = PxCreatePhysics(PX_PHYSICS_VERSION, foundation, tolerance, false); // sin seguimiento de almacenamiento de memoria
	if( !m_sdk ) {
		MessageBox( NULL, L"Fallo en PxCreatePhysics", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Init de extensiones
	if( !PxInitExtensions( *m_sdk ) ) {
		MessageBox( NULL, L"Fallo al crear PxInitExtensions", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Sistema de cocinado
	m_cooking = PxCreateCooking( PX_PHYSICS_VERSION, foundation, PxCookingParams( tolerance ) );
	if( !m_cooking ) {
		MessageBox( NULL, L"Fallo al llamar PxCreateCooking", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	PxSceneDesc sceneDesc( m_sdk->getTolerancesScale() );
	{
		sceneDesc.simulationEventCallback = this; // @TODO 5
		sceneDesc.gravity = PxVec3(0.0f, -980.0f, 0.0f);
		sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
		m_cpuDispatcher = PxDefaultCpuDispatcherCreate( nbThreads );
		if( !m_cpuDispatcher ) {
			MessageBox( NULL, L"PxDefaultCpuDispatcherCreate failed!", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		}
		sceneDesc.cpuDispatcher	= m_cpuDispatcher;
		sceneDesc.filterShader	= ContactFilterShader;
	}
	m_scene = m_sdk->createScene( sceneDesc );
	if( !m_scene ) {
		MessageBox( NULL, L"createScene failed!", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Material por defecto
	m_material = m_sdk->createMaterial( 0.5f, 0.5f, 0.2f );
	if( !m_material ) {
		MessageBox( NULL, L"createMaterial failed!", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Controlador de characters
	m_controllerManager = PxCreateControllerManager( *m_scene ); // @TODO 6
}

/*----------------------------------------------------------------------------------------------------------------------*/

CPhysicsWorld::~CPhysicsWorld( void )
{
	SAFE_RELEASE( m_cooking );
	SAFE_RELEASE( m_scene );
	SAFE_RELEASE( m_material );
	SAFE_RELEASE( m_cpuDispatcher );
	SAFE_RELEASE( m_sdk );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CPhysicsWorld::Update( float timeDiff )
{
	if( m_scene ) {
		float freq = 1.0f / 60.0f;
		m_slotTimer += timeDiff;
		while( m_slotTimer >= freq ) {
			m_slotTimer -= freq;
			m_scene->simulate( freq );
			m_scene->fetchResults( true );
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

CLevel *CPhysicsWorld::AddLevelToPhysicsWorld( void )
{
	CLevel *theLevel = new CLevel();
	// @TODO 2: Carga de malla estática de colisión

		// Cocinado del tema
	PxTriangleMeshDesc tmDesc;
	tmDesc.points.count = theLevel->m_numRenderVertices;
	tmDesc.points.data = theLevel->m_renderVertices;
	tmDesc.points.stride = sizeof( PxVec3 );
	tmDesc.triangles.count = theLevel->m_numRenderIndices / 3;
	tmDesc.triangles.data = theLevel->m_renderIndices;
	tmDesc.triangles.stride = 3 * sizeof( PxU32 );
	PxDefaultMemoryOutputStream buf;
	bool ok = m_cooking->cookTriangleMesh( tmDesc, buf );
	if( !ok ) {
		MessageBox( NULL, L"Error al cocinar la malla de nivel!", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Obtencion de la malla cocinada
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxTriangleMesh* triMesh = m_sdk->createTriangleMesh( input );
	if( !triMesh ) {
		MessageBox( NULL, L"Error obtener la malla cocinada!", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Creacion del actor estatico
	PxRigidStatic *levelActor = m_sdk->createRigidStatic( PxTransform( PxVec3( 0, 0, 0 ) ) );
	if( !levelActor ) {
		MessageBox( NULL, L"Fallo creando el actor de nivel", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Asignamos al actor la geometría de colision
	PxTriangleMeshGeometry geom( triMesh );
	PxShape *levelShape = levelActor->createShape( geom, *m_material );
	if( !levelShape ) {
		MessageBox( NULL, L"Fallo creando el shape de colision del nivel", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Añadimos el actor a la escena
	m_scene->addActor( *levelActor);
	return theLevel;
}

/*----------------------------------------------------------------------------------------------------------------------*/

bool CPhysicsWorld::RayTest( const CVector3D &orig, const CVector3D &dir, float distance, CVector3D &res )
{
	// @TODO 3: Testeo de linea
	const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;
	PxRaycastHit hit;
	bool isCol = m_scene->raycastSingle( PxVec3( orig.x, orig.y, orig.z ), PxVec3( dir.x, dir.y, dir.z ), distance, outputFlags, hit );
	if( isCol ) {
		res = orig + dir * hit.distance;
	}
	return isCol;
}

/*----------------------------------------------------------------------------------------------------------------------*/

CMesh *CPhysicsWorld::AddRigidBodyDynamic( const CVector3D &pos, const CVector3D &iniVel )
{
	CMesh *mesh = new CMesh();

	// @TODO 4: Creación de un objeto dinámico
	//
	// Cocinado
	PxConvexMeshDesc tmDesc;
	tmDesc.points.count = mesh->m_numRenderVertices;
	tmDesc.points.data = mesh->m_renderVertices;
	tmDesc.points.stride = sizeof( PxVec3 );
	tmDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	PxDefaultMemoryOutputStream buf;
	bool ok = m_cooking->cookConvexMesh( tmDesc, buf );
	if( !ok ) {
		MessageBox( NULL, L"Error al cocinar la malla de nivel!", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Obtencion de la malla cocinada
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* triMesh = m_sdk->createConvexMesh( input );
	if( !triMesh ) {
		MessageBox( NULL, L"Error obtener la malla cocinada!", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Creacion del actor dinamico
	PxRigidDynamic *meshActor = m_sdk->createRigidDynamic( PxTransform( PxVec3( pos.x, pos.y, pos.z ) ) );
	meshActor->setSleepThreshold( 1.0f );
	if( !meshActor ) {
		MessageBox( NULL, L"Fallo creando el actor de nivel", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}

	// Asignamos al actor la geometría de colision
	PxConvexMeshGeometry geom( triMesh );
	PxShape *meshShape = meshActor->createShape( geom, *m_material );
	if( !meshShape ) {
		MessageBox( NULL, L"Fallo creando el shape de colision del nivel", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}
	meshActor->setLinearVelocity( PxVec3( iniVel.x, iniVel.y, iniVel.z ) );
	PxRigidBodyExt::updateMassAndInertia( *meshActor, 0.1f );

	// Añadimos el actor a la escena
	m_scene->addActor( *meshActor );
	mesh->SetPhysicsActor( meshActor );
	meshActor->userData = mesh;
	//
	return mesh;
}

/*----------------------------------------------------------------------------------------------------------------------*/
// @TODO 6
CCharDisplacer *CPhysicsWorld::CreateCharDisplacer( const CVector3D &pos )
{
	CCharDisplacer *displacer = new CCharDisplacer();
	PxCapsuleControllerDesc cDesc;
	cDesc.material		= m_material;
	cDesc.position		= PxExtendedVec3( pos.x, pos.y, pos.z );
	cDesc.height		= 150.0f;
	cDesc.radius		= 150.0f;
	cDesc.slopeLimit	= 0;
	cDesc.contactOffset	= 1.0f;
	cDesc.stepOffset	= 0.02f; 
	cDesc.callback		= displacer;
	cDesc.userData = displacer;
	PxCapsuleController *ctrl = static_cast<PxCapsuleController*>( m_controllerManager->createController( *m_sdk, m_scene, cDesc ) );
	displacer->InitPhysXCtrl( ctrl );
	return displacer;
}

/*----------------------------------------------------------------------------------------------------------------------*/

// @TODO TRIGGER
CMesh *CPhysicsWorld::CreateTrigger( const CVector3D &pos, const CVector3D &size ) {
	CMesh *trigger = new CTrigger( pos, size );

	// Cocinado del tema
	PxConvexMeshDesc tmDesc;
	tmDesc.points.count = trigger->m_numRenderVertices;
	tmDesc.points.data = trigger->m_renderVertices;
	tmDesc.points.stride = sizeof( PxVec3 );
	tmDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	PxDefaultMemoryOutputStream buf;
	bool ok = m_cooking->cookConvexMesh( tmDesc, buf );
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* triMesh = m_sdk->createConvexMesh( input );
	PxRigidStatic *meshActor = m_sdk->createRigidStatic( PxTransform( PxVec3( pos.x, pos.y, pos.z ) ) );
	PxConvexMeshGeometry geom( triMesh );
	PxShape *meshShape = meshActor->createShape( geom, *m_material );
	meshShape->setFlag( PxShapeFlag::eSIMULATION_SHAPE, false );
	meshShape->setFlag( PxShapeFlag::eTRIGGER_SHAPE, true );
	m_scene->addActor( *meshActor );
	meshShape->userData = meshActor->userData = trigger;
	return trigger;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CPhysicsWorld::onContact( const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs )
{
	// @TODO 5
}

/*----------------------------------------------------------------------------------------------------------------------*/
// @TODO TRIGGER
void CPhysicsWorld::onTrigger( physx::PxTriggerPair* pairs, physx::PxU32 count )
{
	if( pairs->status != PxPairFlag::eNOTIFY_TOUCH_FOUND && pairs->status != PxPairFlag::eNOTIFY_TOUCH_LOST ) {
		return;
	}
	bool enter = pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND;
	static_cast<CTrigger *>( pairs->triggerShape->userData )->OnTrigger( enter );
}

/*----------------------------------------------------------------------------------------------------------------------*/

