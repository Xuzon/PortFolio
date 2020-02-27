#ifndef BOGLGP_GEOM_H
#define BOGLGP_GEOM_H

#include <cmath>

struct TexCoord
{
    float s, t;
    TexCoord():
    s(0.0f),
    t(0.0f) {}

    TexCoord(float s, float t):
    s(s),
    t(t)
    {
    }
};

struct Color
{
    float r, g, b, a;
    Color(float R, float G, float B, float A):
    r(R),
    g(G),
    b(B),
    a(A)
    {
    }

    Color():
    r(0.0f),
    g(0.0f),
    b(0.0f),
    a(0.0f)
    {

    }
};

struct Vector3
{
    float x, y, z;
    Vector3(float X, float Y, float Z):
    x(X),
    y(Y),
    z(Z) {}

    Vector3():
    x(0.0f),
    y(0.0f),
    z(0.0f) {}

    Vector3(const Vector3& v):
    x(v.x),
    y(v.y),
    z(v.z) {}

    Vector3 operator*(const float s) const
    {
        return Vector3(x * s, y * s, z * s);
    }

    Vector3& operator=(const Vector3& v)
    {
        if (this == &v) {
            return *this;
        }

        x = v.x;
        y = v.y;
        z = v.z;

        return *this;
    }

    Vector3& operator+=(const Vector3& v)
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;

        return *this;
    }

    const Vector3 operator-(const Vector3& v) const
    {
        Vector3 result;
        result.x = x - v.x;
        result.y = y - v.y;
        result.z = z - v.z;

        return result;
    }

    float length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    void normalize()
    {
        float l = 1.0f / length();
        x *= l;
        y *= l;
        z *= l;
    }
};

typedef Vector3 Vertex;

inline float degreesToRadians(const float degrees) {
    const float PIOver180 = 3.14159f / 180.0f;
    return degrees * PIOver180;
}

#endif
