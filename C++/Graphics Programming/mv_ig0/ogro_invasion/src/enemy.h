#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Collider;

class Enemy : public Entity {
public:
    /** Default constructor */
    Enemy(GameWorld* world);

    /** Default destructor */
    virtual ~Enemy();

    virtual Collider* getCollider() { return m_collider; }

    virtual void onCollision(Entity* collider);


    Vector3 getPosition() const { return m_position; }
    void setPosition(const Vector3& pos) { m_position = pos; }
    Vector3 getVelocity() const { return m_velocity; }

    float getYaw() const { return m_yaw; }
    float getPitch() const { return 0.0f; }
    void setYaw(const float yaw) { m_yaw = yaw; }
    void setPitch(const float pitch) {  }

    void kill()
    {
        m_isDead = true;
        onKill();
    }

    void bringToLife()
    {
        m_isDead = false;
        onResurrection();
    }

    bool isDead() const { return m_isDead; }
protected:
    Collider* m_collider;

    Vector3 m_position;
    Vector3 m_velocity;

    float m_yaw;

    bool m_isDead;

private:
    virtual void onKill() = 0;
    virtual void onResurrection() = 0;
};

#endif // ENEMY_H
