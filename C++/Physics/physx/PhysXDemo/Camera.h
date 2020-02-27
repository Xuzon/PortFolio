#ifndef _CAMERA_H_ 
#define _CAMERA_H_

// Includes
#include "Math/Matrix4x4.h"
#include "Math/Vector3D.h"

// Forwards
struct EVENT;

// Definicion de la clase
class CCamera
{
public:

	CCamera( void );
	~CCamera( void );
	void Transform2Dto3D( float x, float y,  CVector3D &vcOrig,  CVector3D &vcDir );
	void Init( unsigned int width, unsigned int height );
	void Update( EVENT &event, float timeDiff );
	void StepLeft( float dist );
	void StepRight( float dist );
	void StepUp( float dist );
	void StepDown( float dist );
	void MoveForward( float dist );
	void MoveBackward( float dist );
	void ApplyToOGL( void );
	inline const CVector3D &GetPos( void ) const { return m_pos; }
	inline const CVector3D &GetDir( void ) const { return m_dir; }
	void SetPos( const CVector3D &p );
	void SetDir( const CVector3D &vc );

	static int LeftCicle;
	static bool LeftClicked;

private:
	void AddRot( float x, float y, float z );
	void CalcViewProjMatrix( void );
	void CalcWorldViewProjMatrix( void );
	void CalcModelViewMatrix( void );
	bool CalcProjMatrix( float fov, float aspect );
	void SetView3D( const CVector3D &right , const CVector3D &up, const CVector3D &dir, const CVector3D &pos );

	unsigned int m_width;
	unsigned int m_height;
	float m_near, m_far;
	CMatrix4x4	m_view,                         // viewmatrix para perspectiva
				m_proj,                          // matriz de proyección perspectiva
				m_world,                          // worldmatrix (en openGL matriz del modelo)
				m_viewProj,                       // combo matrix con view-projection
				m_modelView,                      // combo matrix con model-view
				m_worldViewProj;                  // combo matrix con la transformación total
	CVector3D m_pos, m_right, m_up, m_dir;
	float m_rotX, m_rotY, m_rotZ;
	float m_speed;
	bool m_rightMousePresed;
	bool m_leftMousePresed;
};

#endif