#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

#include "entity.h"

class Player : public Entity
{
    public:
        /** Default constructor */
        Player(GameWorld* const world);

        /** Default destructor */
        virtual ~Player();
        /** Access m_score
         * \return The current value of m_score
         */
        unsigned int getScore() const { return m_score; }
        /** Set m_score
         * \param val New value to set
         */
        void getScore(unsigned int val) { m_score = val; }

        virtual EntityType getType() const { return PLAYER; }

        //At the moment the player doesn't collide with other entities
        virtual Collider* getCollider() { return m_collider; }

        Vector3 getPosition() const { return m_position; }
        void setPosition(const Vector3& pos) {
            m_position = pos;
        }

        Vector3 getVelocity() const { return m_velocity; }

        float getYaw() const { return m_yaw; }
        float getPitch() const { return m_pitch; }
        void setYaw(const float yaw) { m_yaw = yaw; }
        void setPitch(const float pitch) { m_pitch = pitch; }

        void yaw(const float val);
        void pitch(const float val);
        void moveForward(const float speed);

        void increaseScore(int amount) { m_score += amount; }
    private:
        unsigned int m_score; //!< Member variable "m_score"

        virtual void onPrepare(float);
        virtual void onRender() const;
        virtual void onPostRender();
        virtual bool onInitialize();
        virtual void onShutdown();
        virtual void onCollision(Entity* collider) { } //Players don't collide.. yet
        Vector3 m_position;
        Vector3 m_velocity;

        float m_yaw;
        float m_pitch;
        Collider* m_collider;
};

#endif // PLAYER_H
