#ifndef _TRIGGER_H_
#define _TRIGGER_H_
// Includes
#include "Mesh.h"

// Forwards

// Definicion de la clase
class CTrigger : public CMesh
{
public:
	CTrigger( const CVector3D &pos, const CVector3D &size );
	~CTrigger( void );
	void OnTrigger( bool enter );
	
private:
};

#endif