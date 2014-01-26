/** Copyright (C) 2014 Harijs Grinbergs
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

//Based on code from http://ogldev.atspace.co.uk/index.html by Etay Meiri

#ifndef GS_MATH_H
#define	GS_MATH_H

#include "Universal_System/scalar.h"
//#include <stdio.h>
//#include "util.h"

namespace enigma
{

struct Vector2f
{
    gs_scalar x;
    gs_scalar y;

    Vector2f() {};

    Vector2f(gs_scalar x, gs_scalar y) : x(x), y(y) {  };
};


struct Vector3f
{
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;

    Vector3f() {};

    Vector3f(gs_scalar x, gs_scalar y, gs_scalar z) :  x(x), y(y), z(z) {};

    Vector3f& operator+=(const Vector3f& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

    Vector3f& operator-=(const Vector3f& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

    Vector3f& operator*=(gs_scalar f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    Vector3f Cross(const Vector3f& v) const;
    Vector3f& Normalize();
    void Rotate(gs_scalar Angle, const Vector3f& Axis);
};


struct Vector4f
{
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;

    Vector4f() {};

    Vector4f(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar w) : x(x), y(y), z(z), w(w) {};
};

inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z);

    return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z);

    return Ret;
}

inline Vector3f operator*(const Vector3f& l, gs_scalar f)
{
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
}

class Matrix4f
{
public:
    gs_scalar m[4][4];

    Matrix4f() {};

    Matrix4f(gs_scalar a00, gs_scalar a01, gs_scalar a02, gs_scalar a03,
             gs_scalar a10, gs_scalar a11, gs_scalar a12, gs_scalar a13,
             gs_scalar a20, gs_scalar a21, gs_scalar a22, gs_scalar a23,
             gs_scalar a30, gs_scalar a31, gs_scalar a32, gs_scalar a33)
    {
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
        m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }

    void SetZero()
    {
        for (unsigned int i = 0 ; i < 4 ; ++i) {
            for (unsigned int j = 0 ; j < 4 ; ++j) {
                m[i][j] = 0.0;
            }
        }
    }

    Matrix4f Transpose() const
    {
        Matrix4f n;
        for (unsigned int i = 0 ; i < 4 ; ++i) {
            for (unsigned int j = 0 ; j < 4 ; ++j) {
                n.m[i][j] = m[j][i];
            }
        }
        return n;
    }


    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
        Matrix4f Ret;
        for (unsigned int i = 0 ; i < 4 ; ++i) {
            for (unsigned int j = 0 ; j < 4 ; ++j) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }

    Vector4f operator*(const Vector4f& v) const
    {
        Vector4f r;

        r.x = m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z + m[0][3]* v.w;
        r.y = m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z + m[1][3]* v.w;
        r.z = m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z + m[2][3]* v.w;
        r.w = m[3][0]* v.x + m[3][1]* v.y + m[3][2]* v.z + m[3][3]* v.w;

        return r;
    }

    operator gs_scalar*(){
        return &(m[0][0]);
    }

    gs_scalar Determinant() const;

    Matrix4f& Inverse();

    void InitScaleTransform(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ);
    void InitRotateTransform(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    void InitRotateVectorTransform(gs_scalar angle, const Vector3f& vect);
    void InitTranslationTransform(gs_scalar x, gs_scalar y, gs_scalar z);
    void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
    void InitPersProjTransform(gs_scalar fovy, gs_scalar aspect_ratio, gs_scalar znear, gs_scalar zfar);
    void InitOtrhoProjTransform(gs_scalar left, gs_scalar right, gs_scalar bottom, gs_scalar top, gs_scalar znear, gs_scalar zfar);

    void scale(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ);
    void rotate(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    void rotate(gs_scalar angle, gs_scalar x, gs_scalar y, gs_scalar z);
    void translate(gs_scalar x, gs_scalar y, gs_scalar z);
};

struct Quaternion
{
    gs_scalar x, y, z, w;

    Quaternion(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar w) : x(x), y(y), z(z), w(w) {}

    void Normalize();
    Quaternion Conjugate();
 };

Quaternion operator*(const Quaternion& l, const Quaternion& r);
Quaternion operator*(const Quaternion& q, const Vector3f& v);

}
#endif

