#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <cstdlib>
#include "glee/GLee.h"
#include <GL/glu.h>
#include "ogro.h"
#include "glslshader.h"
#include "md2model.h"
#include "collider.h"
#include "gameworld.h"
#include "player.h"
#include "landscape.h"

using std::string;

const string OGRO_MODEL = "data/models/Ogro/tris.md2";
const string OGRO_TEXTURE = "data/models/Ogro/Ogrobase.tga";

Ogro::Ogro(GameWorld* world):
Enemy(world),
m_AIState(OGRO_IDLE),
m_currentTime(0),
m_lastAIChange(0)
{
    string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";
    m_model = new MD2Model(vertexShader, fragmentShader);
    m_model->setAnimation(Animation::IDLE);
}

Ogro::~Ogro()
{
    delete m_model;
}

void Ogro::onPrepare(float dT)
{
    getCollider()->setRadius(m_model->getRadius());

    m_currentTime += dT;

    processAI();

    m_model->update(dT);

    if (m_position.y > 0.0f) {
        m_position.y -= 10.0f * dT;
    }

    Vector3 pos = getPosition();

    float speed = 0.0f;

    if (m_AIState == OGRO_RUNNING)
    {
        speed = 2.0f * dT;
    }
    else if (m_AIState == OGRO_WALK)
    {
        speed = 0.5f * dT;
    }

    float cosYaw = cosf(degreesToRadians(m_yaw));
    float sinYaw = sinf(degreesToRadians(m_yaw));
    pos.x += float(cosYaw) * speed;
    pos.z += float(sinYaw) * speed;

    setPosition(pos);


}

void Ogro::onRender() const
{

    glPushMatrix();
        Vector3 pos = getPosition();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(getYaw(), 0.0f, -1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, m_ogroTextureID);
        m_model->render();
    glPopMatrix();
}

void Ogro::onPostRender()
{

}

bool Ogro::onInitialize()
{
    bool result = m_model->load(OGRO_MODEL);
    if (result)
    {
        if (!m_ogroTexture.load(OGRO_TEXTURE))
        {
            result = false;
        }
        else
        {
            glGenTextures(1, &m_ogroTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_ogroTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_ogroTexture.getWidth(),
                              m_ogroTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                              m_ogroTexture.getImageData());
        }
    }

    m_yaw = (float(rand()) / RAND_MAX) * 360.0f;
    return result;
}

void Ogro::onShutdown()
{

}


AIState getRandomIdleState()
{
    int result = rand() % 3;
    if (result == 0)
    {
        return OGRO_IDLE;
    }
    else if (result == 1)
    {
        return OGRO_CROUCH;
    }
    else
    {
        return OGRO_WALK;
    }
}

void Ogro::processAI()
{
    if (isDead())
    {
        return;
    }

    const float DANGER_DISTANCE = 5.0f;

    Vector3 playerPosition = getWorld()->getPlayer()->getPosition();
    Vector3 playerDirection = getPosition() - playerPosition;
    float playerDistance = playerDirection.length();

    if (playerDistance < DANGER_DISTANCE && m_AIState != OGRO_RUNNING && (m_currentTime - m_lastAIChange) > 3.0f)
    {
        m_model->setAnimation(Animation::RUN);
        m_AIState = OGRO_RUNNING;
        m_lastAIChange = m_currentTime;
    }

    if (playerDistance >= DANGER_DISTANCE)
    {
        if (((m_currentTime + float(rand() % 5) / 10.0f) - m_lastAIChange) > 8.0f)
        {
            AIState newState = getRandomIdleState();
            if (newState != m_AIState)
            {
                m_AIState = newState;
                m_lastAIChange = m_currentTime;
                if (newState == OGRO_IDLE)
                {
                    m_model->setAnimation(Animation::IDLE);
                }
                if (newState == OGRO_CROUCH)
                {
                    m_model->setAnimation(Animation::CROUCH_IDLE);
                    m_yaw += float(rand() % 180) - 90.0f;
                }
                if (newState == OGRO_WALK)
                {
                    m_model->setAnimation(Animation::CROUCH_WALK);
                    m_yaw += float(rand() % 180) - 90.0f;
                }
            }
        }
    }

    //Stop the Ogro's going outside the map bounds
    float minX = getWorld()->getLandscape()->getTerrain()->getMinX() + 2.5f;
    float maxX = getWorld()->getLandscape()->getTerrain()->getMaxX() - 2.5f;
    float minZ = getWorld()->getLandscape()->getTerrain()->getMinZ() + 2.5f;
    float maxZ = getWorld()->getLandscape()->getTerrain()->getMaxZ() - 2.5f;

    float randYaw = 90.0f + (float) (rand() % 90);

    if (getPosition().x < minX ||
        getPosition().x > maxX ||
        getPosition().z < minZ ||
        getPosition().z > maxZ)
    {
        m_yaw += randYaw;
        m_AIState = OGRO_WALK;
        m_model->setAnimation(Animation::RUN);
        m_lastAIChange = m_currentTime;

        if (getPosition().x < minX)
        {
            m_position.x = minX;
        }
        else if (getPosition().x > maxX)
        {
            m_position.x = maxX;
        }
        else if (getPosition().z < minZ)
        {
            m_position.z = minZ;
        }
        else if (getPosition().z > maxZ)
        {
            m_position.z = maxZ;
        }
    }


}

void Ogro::onKill()
{
    int r = (int) rand() % 3;
    if (r == 0)
    {
        m_model->setAnimation(Animation::DEATH1);
    }
    else if (r == 1)
    {
        m_model->setAnimation(Animation::DEATH2);
    }
    else
    {
        m_model->setAnimation(Animation::DEATH3);
    }

    m_AIState = OGRO_DEAD;
}

void Ogro::onResurrection()
{
    m_model->setAnimation(Animation::IDLE);
    m_AIState = OGRO_IDLE;
    m_lastAIChange = m_currentTime;
}
