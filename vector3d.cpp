#include "vector3d.h"
#include <cmath>

namespace gsl
{

    Vector3D::Vector3D(float x_in, float y_in, float z_in) : x{x_in}, y{y_in}, z{z_in}
    {

    }


    Vector3D::Vector3D(const int v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}
    {

    }


    Vector3D::Vector3D(const double v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}
    {

    }


    const Vector3D& Vector3D::operator=(const Vector3D &rhs)
    {
        x = rhs.getX();
        y = rhs.getY();
        z = rhs.getZ();

        return *this;
    }


    Vector3D Vector3D::operator+(const Vector3D &rhs) const
    {
        return {x + rhs.getX(), y + rhs.getY(), z + rhs.getZ()};
    }


    Vector3D Vector3D::operator-(const Vector3D &rhs) const
    {
        return {x - rhs.getX(), y - rhs.getY(), z - rhs.getZ()};
    }


    Vector3D& Vector3D::operator+=(const Vector3D &rhs)
    {
        x += rhs.getX();
        y += rhs.getY();
        z += rhs.getZ();

        return *this;
    }


    Vector3D& Vector3D::operator-=(const Vector3D &rhs)
    {
        x -= rhs.getX();
        y -= rhs.getY();
        z -= rhs.getZ();

        return *this;
    }


    Vector3D Vector3D::operator-() const
    {
        return {-x, -y, -z};
    }


    Vector3D Vector3D::operator*(float rhs) const
    {
        return {x * rhs, y * rhs, z * rhs};
    }


    Vector3D Vector3D::operator^(const Vector3D &rhs) const
        {
            return {y * rhs.getZ() - z * rhs.getY(), z * rhs.getX() - x * rhs.getZ(), x * rhs.getY() - y * rhs.getX()};
    }


    float Vector3D::length() const
    {
        return std::sqrt(std::pow(x, 2.f) + std::pow(y, 2.f) + std::pow(z, 2.f));
    }


    void Vector3D::normalize()
    {
        float l = length();

        if (l > 0.f)
        {
            x = x / l;
            y = y / l;
            z = z / l;
        }
    }


    Vector3D Vector3D::normalized()
    {
        Vector3D normalized;
        float l = length();

        if (l > 0.f)
        {
            normalized.setX(x / l);
            normalized.setY(y / l);
            normalized.setZ(z / l);
        }

        return normalized;
    }


    Vector3D Vector3D::cross(const Vector3D &v1, const Vector3D &v2)
    {
        return {((v1.getY() * v2.getZ()) - (v1.getZ() * v2.getY())), ((v1.getZ() * v2.getX()) - (v1.getX() * v2.getZ())), ((v1.getX() * v2.getY()) - (v1.getY() * v2.getX()))};
    }


    float Vector3D::dot(const Vector3D &v1, const Vector3D &v2)
    {
        return ((v1.getX() * v2.getX()) + (v1.getY() * v2.getY()) + (v1.getZ() * v2.getZ()));
    }


    float *Vector3D::xP()
    {
        return &x;
    }

    float *Vector3D::yP()
    {
        return &y;
    }

    float *Vector3D::zP()
    {
        return &z;
    }


    float Vector3D::getX() const
    {
        return x;
    }


    void Vector3D::setX(const float &value)
    {
        x = value;
    }


    float Vector3D::getY() const
    {
        return y;
    }


    void Vector3D::setY(const float &value)
    {
        y = value;
    }


    float Vector3D::getZ() const
    {
        return z;
    }


    void Vector3D::setZ(const float &value)
    {
        z = value;
    }

} //namespace
