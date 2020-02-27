#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include <list>
#include "uncopyable.h"

class Entity;

class Collider : private Uncopyable {
public:
    Collider(Entity* entity);

    virtual float getRadius() const = 0;
    virtual void setRadius(const float radius) = 0;
    static void updateColliders(std::list<Collider*> &colliders);

    Entity* getEntity() const { return m_entity; }

    virtual ~Collider() { m_entity = NULL; }

private:
    virtual bool collideWith(const Collider* collider) = 0;

    Entity* m_entity;
};

#endif // COLLIDER_H_INCLUDED
