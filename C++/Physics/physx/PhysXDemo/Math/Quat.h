#ifndef _QUAT_H_
#define _QUAT_H_

#include "math.h"
// Our basic quaternion class
class __declspec(dllexport) CQuat {
public:
	float x, y, z, w;

	//---------------------------------------

	CQuat(void) { x=0.0f, y=0.0f, z=0.0f, w=1.0f; }
	CQuat(float _x, float _y, float _z, float _w)
	{ x=_x; y=_y; z=_z; w=_w; }

	void  MakeFromEuler(float fPitch, float fYaw, float fRoll);
	void  Normalize();
	void  Conjugate(CQuat q);
	void  GetEulers(float *fPitch, float *fYaw, float *fRoll) const;
	void  GetMatrix(CMatrix4x4 *m);
	float GetMagnitude(void);


	void    operator /= (float f);
	CQuat operator /  (float f);

	void    operator *= (float f);
	CQuat operator *  (float f) const;

	CQuat operator *  (const CVector3D &v) const;

	CQuat operator *  (const CQuat &q) const;
	void    operator *= (const CQuat &q);

	void    operator += (const CQuat &q);
	CQuat operator +  (const CQuat &q) const;

	CQuat operator~(void) const { return CQuat(-x, -y, -z, w); }

	void Rotate(const CQuat &q1, const CQuat &q2);

	CVector3D Rotate(const CVector3D &v);

	static inline float Dot( const CQuat &q1, const CQuat &q2 ) 
	{ 
		return ( q1.x * q2.x ) + ( q1.y * q2.y ) + ( q1.z * q2.z ) + ( q1.w * q2.w );
	}

	static void Lerp( const CQuat &q1, const CQuat &q2, float t, CQuat &resul ) 
	{ 
		resul = ( ( q1 * ( 1.0f - t ) ) + ( q2 * t ) );
		resul.Normalize();
	}

	static void Slerp( const CQuat &q1, const CQuat &q2, float t, CQuat &resul )
	{
		float dot = Dot( q1, q2 );

		/*	dot = cos(theta)
		if (dot < 0), q1 and q2 are more than 90 degrees apart,
		so we can invert one to reduce spinning	*/
		if( dot < 0 ) {
			dot = -dot;
			resul.Conjugate( q2 );
		} 
		else{
			resul = q2;
		}
		CQuat aux = resul; 
		if ( dot < 0.95f ) {
			float angle = acosf( dot );
			resul =  ( q1 * sinf( angle * ( 1.0f - t ) ) + aux * sinf( angle * t ) ) / sinf( angle );
		}
		else{
			
			// if the angle is small, use linear interpolation								
			Lerp( q1, aux, t, resul );
		}
	}

}; // class

#endif