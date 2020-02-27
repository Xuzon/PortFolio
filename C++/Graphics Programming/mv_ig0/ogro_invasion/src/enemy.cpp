#include "enemy.h"
#include "spherecollider.h"
#include "gameworld.h"
#include "player.h"

Enemy::Enemy(GameWorld* world):
Entity(world),
m_collider(NULL),
m_position(Vector3()),
m_velocity(Vector3()),
m_yaw(0.0f),
m_isDead(false)
{
    //Should probably be moved to onInitialize where we will have access to the model radius
    m_collider = new SphereCollider(this, 0.0f);
}

Enemy::~Enemy()
{
    delete m_collider;
    m_collider = NULL;
}

void Enemy::onCollision(Entity* collider)
{
    //It was a rocket that hit us!
    if (collider->getType() == ROCKET && !isDead())
    {
        getWorld()->getPlayer()->increaseScore(10);
        kill(); //Kill this enemy
    }
}
