#ifndef ROCKET_H_INCLUDED
#define ROCKET_H_INCLUDED

#include "entity.h"
#include "targa.h"

class MD2Model;

class Rocket : public Entity {
public:
    Rocket(GameWorld* const);
    virtual ~Rocket();

    Vector3 getPosition() const { return m_position; }
    void setPosition(const Vector3& pos) { m_position = pos; }

    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }
    void setYaw(const float yaw) { m_yaw = yaw; }
    void setPitch(const float pitch) { m_pitch = pitch; }

    Collider* getCollider() { return m_collider; }

    EntityType getType() const { return ROCKET; }
private:
    virtual void onPrepare(float dt);
    virtual void onRender() const;
    virtual void onPostRender();
    virtual bool onInitialize();
    virtual void onShutdown();
    virtual void onCollision(Entity* collider);

    Vector3 m_position;

    float m_yaw;
    float m_pitch;

    Collider* m_collider;

    MD2Model* m_model;
    unsigned int m_rocketTexID;
    TargaImage m_rocketTexture;
};

#endif // ROCKET_H_INCLUDED
