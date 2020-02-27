#include "entity.h"
#include "collider.h"

using std::list;

Collider::Collider(Entity* entity):
m_entity(entity)
{
}

void Collider::updateColliders(list<Collider*>& colliders)
{
    typedef list<Collider*>::iterator CollisionIterator;
    for(CollisionIterator collider = colliders.begin(); collider != colliders.end(); ++collider)
    {
        //If the attached entity is dead
        if ((*collider)->getEntity()->canBeRemoved())
        {
            continue; //Move to the next one
        }

        for(CollisionIterator other = colliders.begin(); other != colliders.end(); ++other)
        {
            if (*other == *collider || (*other)->getEntity()->canBeRemoved())
            {
                //We are testing the entity against itself, or the other entity is dead
                continue;
            }

            //If we collided
            if ((*collider)->collideWith((*other)))
            {
                //Tell both entities they collided
                (*collider)->getEntity()->collide((*other)->getEntity());
                (*other)->getEntity()->collide((*collider)->getEntity());
            }
        }
    }
}
