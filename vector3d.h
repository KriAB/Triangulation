#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

namespace gsl
{

class Vector3D
{
public:
    //Constructors
    Vector3D(float x_in = 0.f, float y_in = 0.f, float z_in = 0.f);
    Vector3D(const int v);
    Vector3D(const double v);

    //Operators
    const Vector3D& operator=(const Vector3D &rhs);     // v = v
    Vector3D operator+(const Vector3D &rhs) const;      // v + v
    Vector3D operator-(const Vector3D &rhs) const;      // v - v
    Vector3D& operator+=(const Vector3D &rhs);          // v += v
    Vector3D& operator-=(const Vector3D &rhs);          // v -= v
    Vector3D operator-() const;                     // -v
    Vector3D operator*(float rhs) const;          // v * f
    Vector3D operator^(const Vector3D& rhs) const; // v x v  - cross product

    //Functions
    float length() const;
    void normalize();
    Vector3D normalized();
    static Vector3D cross(const Vector3D &v1, const Vector3D &v2);
    static float dot(const Vector3D &v1, const Vector3D &v2);

    //Getters and setters
    float getX() const;
    void setX(const float &value);

    float getY() const;
    void setY(const float &value);

    float getZ() const;
    void setZ(const float &value);

    float *xP();
    float *yP();
    float *zP();


    //Friend functions
    friend std::ostream& operator<<(std::ostream &output, const Vector3D &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y << ", Z = " << rhs.z;
        return output;
    }
    //private:  //Making these public to avoid having to do things like setY(getY()+3) etc.
    float x;
    float y;
    float z;
};

} //namespace

#endif // VECTOR3D_H
