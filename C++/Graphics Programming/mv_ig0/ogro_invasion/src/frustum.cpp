#include <cmath>
#include "glee/GLee.h"
#include "frustum.h"

Plane Frustum::extractPlane(float a, float b, float c, float d)
{
	float t = sqrt( a * a + b * b + c * c );
	Plane p;
	p.a = a / t;
	p.b = b / t;
	p.c = c / t;
	p.d = d / t;
	return p;
}

bool Frustum::sphereInFrustum(float x, float y, float z, float radius)
{
	for (int p = 0; p < 6; p++)
    {
		if (m_planes[p].a * x + 
            m_planes[p].b * y + 
            m_planes[p].c * z + 
            m_planes[p].d <= -radius)
        {
			return false;
        }
    }
	return true;
}

bool Frustum::PointInFrustum( float x, float y, float z )
{
	for (int p = 0; p < 6; p++)
    {
		if (m_planes[p].a * x + 
            m_planes[p].b * y + 
            m_planes[p].c * z + 
            m_planes[p].d < 0)
        {
			return false;
        }
    }
	return true;
}

void Frustum::updateFrustum()
{
    GLfloat projection[16];
    GLfloat modelview[16];
	GLfloat mvp[16];

	/* Get the current PROJECTION and MODELVIEW matrices from OpenGL */
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	glPushMatrix();
    //Load the stored projection matrix
    glLoadMatrixf(projection);
	//multiply the stored MODELVIEW matrix with the projection matrix
	glMultMatrixf(modelview);
	//we read the result of the multiplication
	glGetFloatv(GL_MODELVIEW_MATRIX, mvp);
	//and restore the former MODELVIEW_MATRIX
	glPopMatrix();


	/* Extract the RIGHT plane */
	m_planes[PLANE_RIGHT] = extractPlane(mvp[ 3] - mvp[ 0],
	                                     mvp[ 7] - mvp[ 4],
	                                     mvp[11] - mvp[ 8],
	                                     mvp[15] - mvp[12]);

	/* Extract the LEFT plane */
	m_planes[PLANE_LEFT] = extractPlane(mvp[ 3] + mvp[ 0],
	                                    mvp[ 7] + mvp[ 4],
	                                    mvp[11] + mvp[ 8],
	                                    mvp[15] + mvp[12]);

	/* Extract the TOP plane */
	m_planes[PLANE_TOP] = extractPlane(mvp[ 3] - mvp[ 1],
	                                   mvp[ 7] - mvp[ 5],
	                                   mvp[11] - mvp[ 9],
	                                   mvp[15] - mvp[13]);

	/* Extract the BOTTOM plane */
	m_planes[PLANE_BOTTOM] = extractPlane(mvp[ 3] + mvp[ 1],
	                                      mvp[ 7] + mvp[ 5],
	                                      mvp[11] + mvp[ 9],
	                                      mvp[15] + mvp[13]);

	/* Extract the FAR plane */
	m_planes[PLANE_FAR] = extractPlane(mvp[ 3] - mvp[ 2],
	                                   mvp[ 7] - mvp[ 6],
	                                   mvp[11] - mvp[10],
	                                   mvp[15] - mvp[14]);

	/* Extract the NEAR plane */
	m_planes[PLANE_NEAR] = extractPlane(mvp[ 3] + mvp[ 2],
	                                   mvp[ 7] + mvp[ 6],
	                                   mvp[11] + mvp[10],
	                                   mvp[15] + mvp[14]);

}