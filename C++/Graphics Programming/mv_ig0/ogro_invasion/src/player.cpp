#include "player.h"
#include "gameworld.h"
#include "keyboardinterface.h"
#include "mouseinterface.h"
#include "spherecollider.h"
#include "landscape.h"

Player::Player(GameWorld* const world):
Entity(world),
m_score(0),
m_position(Vector3()),
m_velocity(Vector3()),
m_yaw(0.0f),
m_pitch(0.0f)
{
    m_collider = new SphereCollider(this, 0.75f);
}

Player::~Player()
{
    delete m_collider;
}

void Player::onPrepare(float dT)
{
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_UP) || getWorld()->getKeyboard()->isKeyHeldDown(KC_w))
    {
        moveForward(2.0f * dT);
    }
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_DOWN) || getWorld()->getKeyboard()->isKeyHeldDown(KC_s))
    {
        moveForward(-2.0f * dT);
    }
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_RIGHT))
    {
        yaw(50.0f * dT);
    }
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_LEFT))
    {
        yaw(-50.0f * dT);
    }

    if (getWorld()->getKeyboard()->isKeyPressed(KC_SPACE) ||
        getWorld()->getMouse()->isButtonPressed(0))
    {
        Entity* rocket = getWorld()->spawnEntity(ROCKET);
        rocket->setPosition(getPosition());
        rocket->setYaw(getYaw());
        rocket->setPitch(getPitch());
    }

    float x, y;
    getWorld()->getRelativeMousePos(x, y);

    yaw(float(x) * 40.0f * dT);
    pitch(float(y)* -40.0f * dT);

    m_position.y -= 8.0f * dT;

    float minX = getWorld()->getLandscape()->getTerrain()->getMinX() + 2.5f;
    float maxX = getWorld()->getLandscape()->getTerrain()->getMaxX() - 2.5f;
    float minZ = getWorld()->getLandscape()->getTerrain()->getMinZ() + 2.5f;
    float maxZ = getWorld()->getLandscape()->getTerrain()->getMaxZ() - 2.5f;

    if (m_position.x < minX) m_position.x = minX;
    if (m_position.x > maxX) m_position.x = maxX;
    if (m_position.z < minZ) m_position.z = minZ;
    if (m_position.z > maxZ) m_position.z = maxZ;
}

void Player::onRender() const
{

}

void Player::onPostRender()
{

}

bool Player::onInitialize()
{

    return true;
}

void Player::onShutdown()
{

}

void Player::yaw(const float val)
{
    m_yaw += val;

    if (m_yaw >= 360.0f) m_yaw -= 360.0f;
    if (m_yaw < 0.0f) m_yaw += 360.0f;
}

void Player::pitch(const float val)
{
    m_pitch += val;

    const float PITCH_LIMIT = 45.0f;

    if (m_pitch >= PITCH_LIMIT)
    {
        m_pitch = PITCH_LIMIT;
    }

    if (m_pitch <= -PITCH_LIMIT)
    {
        m_pitch = -PITCH_LIMIT;
    }

}

void Player::moveForward(const float speed)
{
    Vector3 pos = getPosition();

    float cosYaw = cosf(degreesToRadians(m_yaw));
    float sinYaw = sinf(degreesToRadians(m_yaw));
    pos.x += float(cosYaw)*speed;
    pos.z += float(sinYaw)*speed;

    setPosition(pos);
}
