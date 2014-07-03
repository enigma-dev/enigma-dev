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

//Partly based on code from http://ogldev.atspace.co.uk/index.html by Etay Meiri

#ifndef GS_MATH_H
#define	GS_MATH_H

#include "Universal_System/scalar.h"
#include <stdio.h> //For printf
//#include "util.h"

namespace enigma
{

struct Vector2
{
    gs_scalar x;
    gs_scalar y;

    Vector2() {};

    Vector2(gs_scalar x, gs_scalar y) : x(x), y(y) {  };
};


struct Vector3
{
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;

    Vector3() {};

    Vector3(gs_scalar x, gs_scalar y, gs_scalar z) :  x(x), y(y), z(z) {};

    Vector3& operator+=(const Vector3& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

    Vector3& operator-=(const Vector3& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

    Vector3& operator*=(gs_scalar f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    Vector3 Cross(const Vector3& v) const;
    Vector3& Normalize();
    void Rotate(gs_scalar Angle, const Vector3& Axis);
};


struct Vector4
{
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;

    Vector4() {};

    Vector4(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar w) : x(x), y(y), z(z), w(w) {};
};

inline Vector3 operator+(const Vector3& l, const Vector3& r)
{
    Vector3 Ret(l.x + r.x,
                l.y + r.y,
                l.z + r.z);

    return Ret;
}

inline Vector3 operator-(const Vector3& l, const Vector3& r)
{
    Vector3 Ret(l.x - r.x,
                l.y - r.y,
                l.z - r.z);

    return Ret;
}

inline Vector3 operator*(const Vector3& l, gs_scalar f)
{
    Vector3 Ret(l.x * f,
                l.y * f,
                l.z * f);

    return Ret;
}

class Matrix3
{
public:
    gs_scalar m[3][3];

    Matrix3() {};

    Matrix3(gs_scalar a00, gs_scalar a01, gs_scalar a02,
            gs_scalar a10, gs_scalar a11, gs_scalar a12,
            gs_scalar a20, gs_scalar a21, gs_scalar a22)
    {
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
    }

    void SetZero()
    {
        for (unsigned int i = 0 ; i < 3 ; ++i) {
            for (unsigned int j = 0 ; j < 3 ; ++j) {
                m[i][j] = 0.0;
            }
        }
    }

    Matrix3 Transpose() const
    {
        Matrix3 n;
        for (unsigned int i = 0 ; i < 3 ; ++i) {
            for (unsigned int j = 0 ; j < 3 ; ++j) {
                n.m[i][j] = m[j][i];
            }
        }
        return n;
    }

    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
    }

    gs_scalar operator() (unsigned int a, unsigned int b){
        return m[a][b];
    }

    operator gs_scalar*(){
        return &(m[0][0]);
    }

    Vector3 operator*(const Vector3& v) const
    {
        Vector3 r;

        r.x = m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z;
        r.y = m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z;
        r.z = m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z;

        return r;
    }

    inline Matrix3 operator*(const Matrix3& Right) const
    {
        Matrix3 Ret;
        for (unsigned int i = 0 ; i < 3; ++i) {
            for (unsigned int j = 0 ; j < 3; ++j) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }
        return Ret;
    }

    void Print() const
    {
        for (int i = 0 ; i < 3 ; ++i) {
            printf("%f %f %f\n", m[i][0], m[i][1], m[i][2]);
        }
    }

    gs_scalar Determinant() const;
    Matrix3& Inverse();
};

class Matrix4
{
public:
    gs_scalar m[4][4];

    Matrix4() {};

    Matrix4(gs_scalar a00, gs_scalar a01, gs_scalar a02, gs_scalar a03,
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

    Matrix4 Transpose() const
    {
        Matrix4 n;
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

    /*inline Matrix4f operator*(const Matrix4f& n) const
    {
        return Matrix4f(m[0]*n[0]  + m[1]*n[4]  + m[2]*n[8]  + m[3]*n[12],   m[0]*n[1]  + m[1]*n[5]  + m[2]*n[9]  + m[3]*n[13],   m[0]*n[2]  + m[1]*n[6]  + m[2]*n[10]  + m[3]*n[14],   m[0]*n[3]  + m[1]*n[7]  + m[2]*n[11]  + m[3]*n[15],
                       m[4]*n[0]  + m[5]*n[4]  + m[6]*n[8]  + m[7]*n[12],   m[4]*n[1]  + m[5]*n[5]  + m[6]*n[9]  + m[7]*n[13],   m[4]*n[2]  + m[5]*n[6]  + m[6]*n[10]  + m[7]*n[14],   m[4]*n[3]  + m[5]*n[7]  + m[6]*n[11]  + m[7]*n[15],
                       m[8]*n[0]  + m[9]*n[4]  + m[10]*n[8] + m[11]*n[12],  m[8]*n[1]  + m[9]*n[5]  + m[10]*n[9] + m[11]*n[13],  m[8]*n[2]  + m[9]*n[6]  + m[10]*n[10] + m[11]*n[14],  m[8]*n[3]  + m[9]*n[7]  + m[10]*n[11] + m[11]*n[15],
                       m[12]*n[0] + m[13]*n[4] + m[14]*n[8] + m[15]*n[12],  m[12]*n[1] + m[13]*n[5] + m[14]*n[9] + m[15]*n[13],  m[12]*n[2] + m[13]*n[6] + m[14]*n[10] + m[15]*n[14],  m[12]*n[3] + m[13]*n[7] + m[14]*n[11] + m[15]*n[15]);
    }*/

    inline Matrix4 operator*(const Matrix4& Right) const
    {
        Matrix4 Ret;
        for (unsigned int i = 0 ; i < 4; ++i) {
            for (unsigned int j = 0 ; j < 4; ++j) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }
        return Ret;
    }

    Vector4 operator*(const Vector4& v) const
    {
        Vector4 r;

        r.x = m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z + m[0][3]* v.w;
        r.y = m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z + m[1][3]* v.w;
        r.z = m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z + m[2][3]* v.w;
        r.w = m[3][0]* v.x + m[3][1]* v.y + m[3][2]* v.z + m[3][3]* v.w;

        return r;
    }

    gs_scalar operator() (unsigned int a, unsigned int b){
        return m[a][b];
    }

    operator gs_scalar*(){
        return &(m[0][0]);
    }

    void Print() const
    {
        for (int i = 0 ; i < 4 ; ++i) {
            printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
        }
    }

    gs_scalar Determinant() const;

    Matrix4& Inverse();

    void InitScaleTransform(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ);

    void InitRotateTransform(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    //void InitRotateVectorTransform(gs_scalar angle, const Vector3f& vect);
    void InitRotateAxisTransform(gs_scalar angle, gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    void InitRotateXTransform(gs_scalar angle);
    void InitRotateYTransform(gs_scalar angle);
    void InitRotateZTransform(gs_scalar angle);

    void InitTranslationTransform(gs_scalar x, gs_scalar y, gs_scalar z);
    void InitCameraTransform(const Vector3& from, const Vector3& to, const Vector3& up);
    void InitPersProjTransform(gs_scalar fovy, gs_scalar aspect_ratio, gs_scalar znear, gs_scalar zfar);
    void InitOrthoProjTransform(gs_scalar left, gs_scalar right, gs_scalar bottom, gs_scalar top, gs_scalar znear, gs_scalar zfar);

    void scale(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ);

    void rotate(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    void rotate(gs_scalar angle, gs_scalar x, gs_scalar y, gs_scalar z);
    void rotateX(gs_scalar angle);
    void rotateY(gs_scalar angle);
    void rotateZ(gs_scalar angle);

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
Quaternion operator*(const Quaternion& q, const Vector3& v);

}
#endif

