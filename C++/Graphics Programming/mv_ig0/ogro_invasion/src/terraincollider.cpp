#ifdef _WIN32
#include <windows.h>
#endif

#include "terraincollider.h"
#include "terrain.h"
#include "entity.h"

TerrainCollider::TerrainCollider(Entity* entity, Terrain* terrain):
Collider(entity),
m_terrain(terrain)
{

}

bool TerrainCollider::collideWith(const Collider* collider)
{
    /*
        If the collider is below the terrain we class it as a
        collision we move the entity up to be level with the terrain
        and return true in this case.
    */


    Vector3 position = collider->getEntity()->getPosition();
    float height = m_terrain->getHeightAt(position.x, position.z);
    float radius = collider->getRadius();

    if (position.y < (height + radius)) {
    //    position.y = (height + radius);
        //Move the entity up level with the terrain
      //  collider->getEntity()->setPosition(position);
        return true;
    }

    return false;
}
