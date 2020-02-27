
#ifndef _BALL_H_
#define _BALL_H_

// includes
#include "../Math/Vector3D.h"
#include "Physics.h"


// forwards

// Definicion de la clase
class CBall : ICircleSegmentContactReport
{

public:

	// Constructor / destructor
	CBall( int resX, int resY, const CVector3D &pos );
	~CBall( void );

	// Pintado
	void Render( void );
private:

	// Radio de circulo
	float m_radius;

	// Posicion
	CVector3D m_pos;

	// Resolucion de pantalla
	int m_resX, m_resY;

	// Puntero al actor de fisicas
	CPhysics::PhysicActor *m_physicsActor;

	CVector3D color = CVector3D(0.f, 0.f, 1.f);

	// Heredado vía ICircleSegmentContactReport
	virtual void OnContact() override;
};

#endif