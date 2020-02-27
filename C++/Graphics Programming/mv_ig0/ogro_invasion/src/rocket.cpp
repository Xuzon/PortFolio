#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "rocket.h"
#include "gameworld.h"
#include "spherecollider.h"
#include "md2model.h"
#include "glslshader.h"

using std::string;

const string ROCKET_MODEL = "data/models/Rocket/rocket.md2";
const string ROCKET_TEXTURE = "data/models/Rocket/rocket.tga";

Rocket::Rocket(GameWorld* world):
Entity(world),
m_collider(NULL),
m_model(NULL)
{
    m_collider = new SphereCollider(this, 0.0f);

    string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";
    m_model = new MD2Model(vertexShader, fragmentShader);
    m_model->setAnimation(Animation::IDLE);
}

Rocket::~Rocket()
{
    delete m_collider;
    delete m_model;
}

void Rocket::onPrepare(float dT)
{
    getCollider()->setRadius(m_model->getRadius());

    Vector3 velocity;

    float cosYaw = cosf(degreesToRadians(m_yaw));
    float sinYaw = sinf(degreesToRadians(m_yaw));
    float sinPitch = sinf(degreesToRadians(m_pitch));
    float cosPitch = cosf(degreesToRadians(m_pitch));

    const float speed = 20.0f;

    velocity.x = cosPitch * cosYaw * speed;
    velocity.y = sinPitch * speed;
    velocity.z = cosPitch * sinYaw * speed;

    const Vector3 gravity(0.0f, -1.0f, 0.0f);

    m_position += velocity * dT;
   // m_position += gravity * dT;
}

void Rocket::onRender() const
{
    glPushMatrix();
        Vector3 pos = getPosition();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(getYaw(), 0.0f, -1.0f, 0.0f);
        glRotatef(getPitch(), 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, m_rocketTexID);
        glScalef(0.5f, 0.5f, 0.5f);
        m_model->render();
    glPopMatrix();
}

void Rocket::onPostRender()
{

}

bool Rocket::onInitialize()
{
    bool result = m_model->load(ROCKET_MODEL);
    if (result)
    {
        if (!m_rocketTexture.load(ROCKET_TEXTURE))
        {
            result = false;
        }
        else
        {
            glGenTextures(1, &m_rocketTexID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_rocketTexID);
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_rocketTexture.getWidth(),
                              m_rocketTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                              m_rocketTexture.getImageData());
        }
    }

    return result;
}

void Rocket::onShutdown()
{

}

void Rocket::onCollision(Entity* collider)
{
    if (collider->getType() == PLAYER) return;

    //Create an explosion where the rocket is
    Entity* explosion = getWorld()->spawnEntity(EXPLOSION);
    explosion->setPosition(getPosition());

    destroy(); //Mark the rocket as dead
}
