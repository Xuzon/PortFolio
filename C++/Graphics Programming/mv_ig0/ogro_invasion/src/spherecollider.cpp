#include "entity.h"
#include "geom.h"
#include "spherecollider.h"

SphereCollider::SphereCollider(Entity* entity, float radius):
Collider(entity),
m_radius(radius)
{
    //ctor
}

SphereCollider::~SphereCollider()
{
    //dtor
}

bool SphereCollider::collideWith(const Collider* collider)
{
    Vector3 entityPosition = getEntity()->getPosition();
    float entityRadius = getRadius();

    Vector3 otherPosition = collider->getEntity()->getPosition();
    float otherRadius = collider->getRadius();

    //If the distance between the 2 center points is less than the 2 radii combined
    //then this is a collision
    if ((entityPosition - otherPosition).length() <= (entityRadius + otherRadius))
    {
        return true;
    }

    return false;
}
