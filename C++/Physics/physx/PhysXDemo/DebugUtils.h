
// Include
#include "Math/Vector3D.h"

// Forwards
class CMatrix4x4;

// defines
#define MAX_DEBUG_PRIMITIVES 1000

// Pinta capsula
void DebugRenderCapsule( CMatrix4x4 &matrix, float height, float radius, const CVector3D &color );

// Pinta linea
void DebugRenderLine3D( const CVector3D &p1, const CVector3D &p2, const CVector3D &color );

// Pinta triángulo
void DebugRenderTriangle( const CVector3D &p1, const CVector3D &p2, const CVector3D &p3, bool edges, const CVector3D &color );

// Pinta esfera
void DebugRenderSphere( const CVector3D &pos, float radius, const CVector3D &color, int divV, int divH );

/*----------------------------------------------------------------------------------------------------------------------*/

class IDebugRenderable
{
public:
	IDebugRenderable( void );
	virtual ~IDebugRenderable( void );
	void Update( float timeDiff );
	virtual void Render( void ) = 0;
	float m_timeLeft;
	bool m_mustDel;
	CVector3D m_color;
};

/*----------------------------------------------------------------------------------------------------------------------*/

class CDebugSphere : public IDebugRenderable
{
public:
	CDebugSphere( void );
	~CDebugSphere( void );
	void Render( void );
	CVector3D m_pos;
	float m_radius;
};

/*----------------------------------------------------------------------------------------------------------------------*/

class CDebugLine : public IDebugRenderable
{
public:
	CDebugLine( void );
	~CDebugLine( void );
	void Render( void );
	CVector3D m_points[2];
};

/*----------------------------------------------------------------------------------------------------------------------*/

class CDebugUtilManager 
{
public:
	static CDebugUtilManager *GetInstance( void );
	static void Destroy( void );
	void Update( float timeDiff );
	void Render( void );
	void CreateSphere( const CVector3D &pos, float radius, const CVector3D &color, float time=0.5f );
	void CreateLine( const CVector3D &p0, const CVector3D &p1, const CVector3D &color, float time=0.5f );
private:
	CDebugUtilManager( void );
	~CDebugUtilManager( void );
	static CDebugUtilManager *m_instance;
	IDebugRenderable *m_primitives[MAX_DEBUG_PRIMITIVES];
};

/*----------------------------------------------------------------------------------------------------------------------*/