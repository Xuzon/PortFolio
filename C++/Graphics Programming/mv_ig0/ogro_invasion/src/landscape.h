#ifndef LANDSCAPE_H_INCLUDED
#define LANDSCAPE_H_INCLUDED

#include <stdexcept>
#include <memory>

#include "entity.h"
#include "terrain.h"

class Collider;

class Landscape : public Entity {
private:
    Terrain m_terrain;
    std::auto_ptr<Collider> m_collider;
    std::string m_heightmap;

    virtual bool onInitialize();
    virtual void onRender() const;
    virtual void onShutdown();
    virtual void onCollision(Entity* entity);
    virtual void onPrepare(float dT);
    virtual void onPostRender();

public:
    Landscape(GameWorld* world, const std::string& heighMap, const std::string& vertexShader,
              const std::string& fragmentShader, const std::string& waterVert, const std::string& waterFrag);
    virtual ~Landscape();

    virtual Collider* getCollider() { return m_collider.get(); }

    Vector3 getPosition() const { return Vector3(0.0f, 0.0f, 0.0f); }
    void setPosition(const Vector3& v) { throw std::logic_error("Attempted to move the landscape"); }
    Vector3 getVelocity() const { return Vector3(0.0f, 0.0f, 0.0f); }
    float getYaw() const { return 0.0f; }
    float getPitch() const { return 0.0f; }
    void setYaw(const float yaw) { }
    void setPitch(const float pitch) { }

    EntityType getType() const { return LANDSCAPE; }
    Terrain* getTerrain() { return &m_terrain; }
};


#endif // MAP_H_INCLUDED
