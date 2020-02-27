#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "collider.h"

class SphereCollider : public Collider
{
    public:
        /** Default constructor */
        SphereCollider(Entity* entity, float radius);

        /** Default destructor */
        virtual ~SphereCollider();

        virtual float getRadius() const { return m_radius; }
        void setRadius(const float radius) { m_radius = radius; }

    private:
        bool collideWith(const Collider* collider);

        float m_radius;
};

#endif // SPHERECOLLIDER_H
