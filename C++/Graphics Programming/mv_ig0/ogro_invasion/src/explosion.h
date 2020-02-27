#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED

#include <vector>
#include <memory>

#include "glee/GLee.h"
#include "entity.h"
#include "geom.h"
#include "targa.h"

class GLSLProgram;

class Explosion : public Entity
{
public:
    Explosion(GameWorld* world, unsigned int numParticles);
    virtual ~Explosion();

    struct Particle
    {
        float life;
        Vector3 position;
        Vector3 velocity;
        Color color;
    };

    typedef std::vector<Particle>::iterator ParticleIterator;
    typedef std::vector<Particle>::const_iterator ConstParticleIterator;

    Vector3 getPosition() const { return m_position; }
    void setPosition(const Vector3& v)
    {
        m_position = v;
        for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it)
        {
            (*it).position = m_position;
        }
    }

    float getYaw() const { return 0.0f; }
    float getPitch() const { return 0.0f; }
    void setYaw(const float v) { }
    void setPitch(const float v) { }

    Collider* getCollider() { return NULL; }

    EntityType getType() const { return EXPLOSION; }
private:
    void onPrepare(float dt);
    void onRender() const;
    void onPostRender();
    bool onInitialize();
    void onCollision(Entity* collider) {}
    void onShutdown();

    std::vector<Particle> m_particles;

    static GLuint m_vertexBuffer;
    static GLuint m_colorBuffer;
    static std::auto_ptr<GLSLProgram> m_shaderProgram;

    static TargaImage m_particleTexture;
    static GLuint m_particleTexID;

    Vector3 m_position;
};


#endif // EXPLOSION_H_INCLUDED
