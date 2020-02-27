#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "uncopyable.h"
#include "geom.h"

class Entity;

class Camera : private Uncopyable {
public:
    Camera(const Vector3& position);
    Camera();

    void setPosition(const Vector3& position);
    void yaw(const float degrees);
    void pitch(const float degrees);
    void apply();

    void attachTo(Entity* entity)
    {
        m_attachedEntity = entity;
    }
private:
    Vector3 m_position;
    Vector3 m_up;
    Vector3 m_right;
    Vector3 m_forward;
    Vector3 m_lookAt;

    float m_yaw;
    float m_pitch;

    Entity* m_attachedEntity;
};

#endif // CAMERA_H_INCLUDED
