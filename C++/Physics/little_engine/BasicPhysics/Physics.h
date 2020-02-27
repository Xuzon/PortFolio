#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "../Math/Vector3D.h"

#define MAX_DYNAMICS 1000

class ICircleSegmentContactReport
{
public:
	virtual void OnContact() = 0;
};

// Definicion de segmento
struct sSegment
{
	CVector3D p[2];

	CVector3D GetNormal() const
	{
		CVector3D u = p[0] - p[1];
		u.Normalize();
		return CVector3D(-u.y, u.x, 0.0f);
	}
};

// Definicion de material
struct sMaterial
{
	float restitution;
};


// Definicion de la clase
class CPhysics
{

public:

	// Actor de fisicas
	struct PhysicActor
	{
		CVector3D vel;
		CVector3D pos;
		sMaterial mat;
		bool sleeping = false;
		float accumulatedMovement = 0;
		float timeDeltaCheckSleeping = 0;
		ICircleSegmentContactReport* report = nullptr;
	};

	// Geometria de segmentos para fisicas
	struct sStaticGeometrySegments
	{
		unsigned int numSegments;
		sSegment *segments;
	};

	// Geometria de circulos para fisicas
	struct sDynamicGeometryCircle
	{
		float radius;
		PhysicActor actorInfo;
	};

	// Constructor / destructor
	CPhysics( const CVector3D &gravity );
	~CPhysics( void );

	// Upadate de fisicas
	void Update( float timeDiff );
	void UpdateDymanicPos( sDynamicGeometryCircle &geom, float timeDiff );

	// Colision segmento circulo
	bool CheckCircleSegmentCollision( const CVector3D &circlePos, float circleRadius, const sSegment &segment, CVector3D &col, CVector3D &normal );
	bool CheckSegmentToSegmentCollision( const sSegment &s0, const sSegment &s1, CVector3D &resul );
	bool CheckCircleCircleCollision(const CVector3D& circle1Pos, float circle1Radius, 
		const CVector3D& circle2Pos, float circle2Radius, CVector3D& col, CVector3D& normal);

	// Pintado
	void Render( void );

	// Añadimos actor dinamico
	PhysicActor *AddDynamicActor( const CVector3D &pos, float radius );

	// Añadimos actor estatico
	void AddStaticActor( sSegment *segments, unsigned int numSegments );
private:

	float minDeltaDispl = 0.01f;
	// Propiedades del mundo fisico
	CVector3D m_gravityForce;

	// Material por defecto
	sMaterial m_defaultMaterial;

	// Listado de actores
	unsigned int m_numStatic;
	sStaticGeometrySegments *m_staticActors[100];
	unsigned int m_numDynamic;
	sDynamicGeometryCircle *m_dynamicActors[MAX_DYNAMICS];
};

#endif