#ifndef TERRAINCOLLIDER_H_INCLUDED
#define TERRAINCOLLIDER_H_INCLUDED

#include "collider.h"

class Terrain;

class TerrainCollider : public Collider {
private:
    bool collideWith(const Collider* collider);

    Terrain* m_terrain;
public:
    TerrainCollider(Entity* entity, Terrain* terrain);

    float getRadius() const { return 0.0f; } //The terrain doesn't collide with other entities in the same way
    void setRadius(const float radius) { }
};

#endif // TERRAINCOLLIDER_H_INCLUDED
