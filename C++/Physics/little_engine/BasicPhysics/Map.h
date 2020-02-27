
#ifndef _MAP_H_
#define _MAP_H_

#include "../Math/Vector3D.h"
#include "Physics.h"

// Definicion de la clase
class CMap
{

public:
	CMap( int resX, int resY );
	~CMap( void );
	void Render( void );
	void AddSegment( const CVector3D &p0, const CVector3D &p1 );
private:

	sSegment m_segments[100];
	unsigned int m_numSegments;
	int m_resX, m_resY;
};

#endif