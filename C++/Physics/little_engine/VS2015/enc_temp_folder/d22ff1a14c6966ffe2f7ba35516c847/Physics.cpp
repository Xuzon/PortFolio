
#include "Physics.h"
#include <stdlib.h>

__inline float Randf( float min, float max )
{
	return (float)( ((rand()&32767)*(1.0/32767.0))*(max-min) + min );
}

/*----------------------------------------------------------------------------------------------------------------------*/

CPhysics::CPhysics( const CVector3D &gravity )
{
	m_gravityForce = gravity;
	m_defaultMaterial.restitution = 0.7f;
	m_numStatic = 0;
	m_numDynamic = 0;
}

/*----------------------------------------------------------------------------------------------------------------------*/

CPhysics::~CPhysics( void )
{
	for( unsigned int i=0; i<m_numStatic; ++i ) {
		delete [] m_staticActors[i]->segments;
		delete m_staticActors[i];
	}
	for( unsigned int i=0; i<m_numDynamic; ++i ) {
		delete m_dynamicActors[i];
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CPhysics::Update( float timeDiff )
{
	for( unsigned int i=0; i<m_numDynamic; ++i ) {
		UpdateDymanicPos( *m_dynamicActors[i], timeDiff );
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CPhysics::UpdateDymanicPos( sDynamicGeometryCircle &geom, float timeDiff )
{
	// @TODO_BP 1, @TODO_BP 2, @TODO_BP 3, @TODO_BP 4
	geom.actorInfo.vel += m_gravityForce * timeDiff;
	CVector3D newPos = geom.actorInfo.pos + geom.actorInfo.vel * timeDiff;
	for (unsigned int i = 0; i < m_numStatic; ++i)
	{
		sStaticGeometrySegments* staticActor = m_staticActors[i];
		for (int j = 0; j < staticActor->numSegments; ++j)
		{
			CVector3D validPos;
			CVector3D normal;
			if (CheckCircleSegmentCollision(newPos, geom.radius, staticActor->segments[j], validPos, normal))
			{
				newPos = validPos;
				break;
			}
		}
	}
	geom.actorInfo.pos = newPos;
}

/*----------------------------------------------------------------------------------------------------------------------*/

bool CPhysics::CheckCircleSegmentCollision( const CVector3D &circlePos, float circleRadius, const sSegment &segment, CVector3D &col, CVector3D &normal )
{
	CVector3D segV = segment.p[1] - segment.p[0];
	CVector3D ptV = circlePos - segment.p[0];

	// Calculamos el punto mas cercano desde el centro del circulo al segmento
	// proyectamos ptV sobre segV
	float projVMod = ptV * ( segV / segV.GetMagnitude() );

	// projVMod es la magnitud del vector proyeccion
	CVector3D closestPoint;
	if( projVMod < 0 ) {
		closestPoint = segment.p[0];
	}
	else if( projVMod > segV.GetMagnitude() ) {
		closestPoint = segment.p[1];
	}
	else {
		// relativo a p[0]
		CVector3D projV = ( segV / segV.GetMagnitude() ) * projVMod;

		// relativo a coordenadas de mundo
		closestPoint = segment.p[0] + projV;
	}
	
	// Vector al centro del circulo
	CVector3D distV = circlePos - closestPoint;

	// Si la magnitud es menor que el radio, hay colision
	float distVMod = distV.GetMagnitude();
	if( distVMod < circleRadius ) {
		normal = distV;
		normal.Normalize();
		col = circlePos + ( normal * ( circleRadius - distVMod ) );
		return true;
	}
	return false;
}

/*----------------------------------------------------------------------------------------------------------------------*/

bool CPhysics::CheckSegmentToSegmentCollision( const sSegment &s0, const sSegment &s1, CVector3D &resul )
{
	float x1_ = s0.p[0].x;
	float x2_ = s0.p[1].x;
	float y1_ = s0.p[0].y;
	float y2_ = s0.p[1].y;
	float x3_ = s1.p[0].x;
	float x4_ = s1.p[1].x;
	float y3_ = s1.p[0].y;
	float y4_ = s1.p[1].y;
	float r, s, d;

	// Si no son paralelas...
	if ((y2_ - y1_) / (x2_ - x1_) != (y4_ - y3_) / (x4_ - x3_))
	{
		d = (((x2_ - x1_) * (y4_ - y3_)) - (y2_ - y1_) * (x4_ - x3_));
		if (d != 0)
		{
			r = (((y1_ - y3_) * (x4_ - x3_)) - (x1_ - x3_) * (y4_ - y3_)) / d;
			s = (((y1_ - y3_) * (x2_ - x1_)) - (x1_ - x3_) * (y2_ - y1_)) / d;
			if (r >= 0 && r <= 1)
			{
				if (s >= 0 && s <= 1)
				{
					resul.x = x1_ + r * (x2_ - x1_);
					resul.y = y1_ + r * (y2_ - y1_);
					return true;
				}
			}
		}
	}
	return false;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CPhysics::Render( void )
{
}

/*----------------------------------------------------------------------------------------------------------------------*/

CPhysics::PhysicActor *CPhysics::AddDynamicActor( const CVector3D &pos, float radius )
{
	if( m_numDynamic < MAX_DYNAMICS ) {
		sDynamicGeometryCircle *geom = new sDynamicGeometryCircle;
		geom->actorInfo.pos = pos;
		geom->actorInfo.vel = CVector3D( 0.0f, 0.0f, 0.0f );
		geom->actorInfo.mat = m_defaultMaterial;
		geom->radius = radius;
		m_dynamicActors[m_numDynamic++] = geom;
		return &geom->actorInfo;
	}
	return 0;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CPhysics::AddStaticActor( sSegment *segments, unsigned int numSegments )
{
	sStaticGeometrySegments *geom = new sStaticGeometrySegments;
	geom->numSegments = numSegments;
	geom->segments = new sSegment[numSegments];
	for( unsigned int i=0; i<numSegments; ++i ) {
		geom->segments[i] = segments[i];
	}
	m_staticActors[m_numStatic++] = geom;
}

/*----------------------------------------------------------------------------------------------------------------------*/
