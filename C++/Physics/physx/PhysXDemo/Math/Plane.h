#ifndef _PLANE_H_
#define _PLANE_H_

#include "Vector3D.h"

// Our basic plane class
class CPolygon;
class CRay;
class CAabb;
class CObb;
class __declspec(dllexport) CPlane {
public:
	CVector3D m_vcN,       // plane normal vector
		m_vcPoint;   // point on plane
	float     m_fD;        // distance to origin

	//---------------------------------------

	CPlane(void) { /* nothing to do */ ; }

	inline void  Set(const CVector3D &vcN, const CVector3D &vcP);
	inline void  Set(const CVector3D &vcN, const CVector3D &vcP, float fD);
	inline void  Set(const CVector3D &v0,  const CVector3D &v1, const CVector3D &v2);
	inline float Distance(const CVector3D &vcPoint);
	inline int   Classify(const CVector3D &vcPoint);
	int   Classify(const CPolygon &Polygon);

	bool Clip(const CRay*, float, CRay*, CRay*);

	bool Intersects(const CVector3D &vc0, const CVector3D &vc1, 
		const CVector3D &vc2);
	bool Intersects(const CPlane &plane, CRay *pIntersection);
	bool Intersects(const CAabb &aabb);
	bool Intersects(const CObb &obb);

}; // class

#endif