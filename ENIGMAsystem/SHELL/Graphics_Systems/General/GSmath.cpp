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

//#include <stdlib.h>
//#include <assert.h>
#include <cmath> //For cos, sin and tan
#include "GSmath.h"

#define M_PI		3.14159265358979323846

namespace enigma
{

#define cosd(x)  ::cos(x * M_PI / 180.0)
#define sind(x)  ::sin(x * M_PI / 180.0)
#define degtorad(x) x*(M_PI/180.0)

///////////
//Vector3//
///////////
Vector3 Vector3::Cross(const Vector3& v) const
{
    const gs_scalar _x = y * v.z - z * v.y;
    const gs_scalar _y = z * v.x - x * v.z;
    const gs_scalar _z = x * v.y - y * v.x;

    return Vector3(_x, _y, _z);
}

Vector3& Vector3::Normalize()
{
    const gs_scalar Length = sqrt(x * x + y * y + z * z);

    x /= Length;
    y /= Length;
    z /= Length;

    return *this;
}

void Vector3::Rotate(gs_scalar Angle, const Vector3& Axe)
{
    const gs_scalar SinHalfAngle = sind(Angle/2.0);
    const gs_scalar CosHalfAngle = cosd(Angle/2.0);

    const gs_scalar Rx = Axe.x * SinHalfAngle;
    const gs_scalar Ry = Axe.y * SinHalfAngle;
    const gs_scalar Rz = Axe.z * SinHalfAngle;
    const gs_scalar Rw = CosHalfAngle;
    Quaternion RotationQ(Rx, Ry, Rz, Rw);

    Quaternion ConjugateQ = RotationQ.Conjugate();
  //  ConjugateQ.Normalize();
    Quaternion W = RotationQ * (*this) * ConjugateQ;

    x = W.x;
    y = W.y;
    z = W.z;
}

///////////
//MATRIX3//
///////////
gs_scalar Matrix3::Determinant() const
{
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

Matrix3& Matrix3::Inverse()
{
    gs_scalar determinant, invDeterminant;
    gs_scalar tmp[9];

    tmp[0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    tmp[1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
    tmp[2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    tmp[3] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
    tmp[4] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
    tmp[5] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
    tmp[6] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
    tmp[7] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
    tmp[8] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

    // check determinant if it is 0
    determinant = m[0][0] * tmp[0] + m[0][1] * tmp[3] + m[0][2] * tmp[6];
    if(fabs(determinant) <= 0.00001f)  //THIS SHOULD PROBABLY ERROR OUT
    {
        printf("Cannot do inverse for the 3x3 matrix!\n");
        return *this;
    }

    // divide by the determinant
    invDeterminant = 1.0f / determinant;
    m[0][0] = invDeterminant * tmp[0];
    m[0][1] = invDeterminant * tmp[1];
    m[0][2] = invDeterminant * tmp[2];
    m[1][0] = invDeterminant * tmp[3];
    m[1][1] = invDeterminant * tmp[4];
    m[1][2] = invDeterminant * tmp[5];
    m[2][0] = invDeterminant * tmp[6];
    m[2][1] = invDeterminant * tmp[7];
    m[2][2] = invDeterminant * tmp[8];

    return *this;
}

///////////
//MATRIX4//
///////////
void Matrix4::scale(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ)
{
    m[0][0] = m[0][0]*ScaleX;  m[0][1] = m[0][1]*ScaleX;  m[0][2] = m[0][2]*ScaleX;  m[0][3] = m[0][3]*ScaleX;
    m[1][0] = m[1][0]*ScaleY;  m[1][1] = m[1][1]*ScaleY;  m[1][2] = m[1][2]*ScaleY;  m[1][3] = m[1][3]*ScaleY;
    m[2][0] = m[2][0]*ScaleZ;  m[2][1] = m[2][1]*ScaleZ;  m[2][2] = m[2][2]*ScaleZ;  m[2][3] = m[2][3]*ScaleZ;
}

//This 2 could techincally be expanded like others so they would be faster
//By a vector
void Matrix4::rotate(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ)
{
    Matrix4 sm;
    sm.InitRotateTransform(RotateX, RotateY, RotateZ);
    *this = sm*(*this);
}

//Around an axis
void Matrix4::rotate(gs_scalar angle, gs_scalar x, gs_scalar y, gs_scalar z)
{
    Matrix4 sm;
    sm.InitRotateAxisTransform(angle, x, y, z);
    *this = sm*(*this);
}

void Matrix4::rotateX(gs_scalar angle)
{
    gs_scalar c = cosd(angle);
    gs_scalar s = sind(angle);
    gs_scalar m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3],
              m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];

    m[1][0] = m10 * c + m20 *-s;
    m[1][1] = m11 * c + m21 *-s;
    m[1][2] = m12 * c + m22 *-s;
    m[1][3] = m13 * c + m23 *-s;
    m[2][0] = m10 * s + m20 * c;
    m[2][1] = m11 * s + m21 * c;
    m[2][2] = m12 * s + m22 * c;
    m[2][3] = m13 * s + m23 * c;
}

void Matrix4::rotateY(gs_scalar angle)
{
    gs_scalar c = cosd(angle);
    gs_scalar s = sind(angle);
    gs_scalar m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3],
              m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];

    m[0][0] = m00 * c + m20 * s;
    m[0][1] = m01 * c + m21 * s;
    m[0][2] = m02 * c + m22 * s;
    m[0][3] = m03 * c + m23 * s;
    m[2][0] = m00 *-s + m20 * c;
    m[2][1] = m01 *-s + m21 * c;
    m[2][2] = m02 *-s + m22 * c;
    m[2][3] = m03 *-s + m23 * c;
}

void Matrix4::rotateZ(gs_scalar angle)
{
    gs_scalar c = cosd(angle);
    gs_scalar s = sind(angle);
    gs_scalar m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3],
              m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];

    m[0][0] = m00 * c + m10 *-s;
    m[0][1] = m01 * c + m11 *-s;
    m[0][2] = m02 * c + m12 *-s;
    m[0][3] = m03 * c + m13 *-s;
    m[1][0] = m00 * s + m10 * c;
    m[1][1] = m01 * s + m11 * c;
    m[1][2] = m02 * s + m12 * c;
    m[1][3] = m03 * s + m13 * c;
}

void Matrix4::translate(gs_scalar x, gs_scalar y, gs_scalar z)
{
    m[0][0] += m[3][0]*x; m[0][1] += m[3][1]*x; m[0][2] += m[3][2]*x; m[0][3] += m[3][3]*x;
    m[1][0] += m[3][0]*y; m[1][1] += m[3][1]*y; m[1][2] += m[3][2]*y; m[1][3] += m[3][3]*y;
    m[2][0] += m[3][0]*z; m[2][1] += m[3][1]*z; m[2][2] += m[3][2]*z; m[2][3] += m[3][3]*z;
}

void Matrix4::InitScaleTransform(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ)
{
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

void Matrix4::InitRotateTransform(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ)
{
    Matrix4 rx, ry, rz;
    const gs_scalar x = degtorad(RotateX);
    const gs_scalar y = degtorad(RotateY);
    const gs_scalar z = degtorad(RotateZ);

    const gs_scalar cx = cos(x);
    const gs_scalar cy = cos(y);
    const gs_scalar cz = cos(z);

    const gs_scalar sx = sin(x);
    const gs_scalar sy = sin(y);
    const gs_scalar sz = sin(z);

    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f; rx.m[0][2] = 0.0f; rx.m[0][3] = 0.0f;
    rx.m[1][0] = 0.0f; rx.m[1][1] = cx;   rx.m[1][2] = -sx;  rx.m[1][3] = 0.0f;
    rx.m[2][0] = 0.0f; rx.m[2][1] = sx;   rx.m[2][2] = cx;   rx.m[2][3] = 0.0f;
    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f; rx.m[3][2] = 0.0f; rx.m[3][3] = 1.0f;

    ry.m[0][0] = cy;   ry.m[0][1] = 0.0f; ry.m[0][2] = -sy;  ry.m[0][3] = 0.0f;
    ry.m[1][0] = 0.0f; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f; ry.m[1][3] = 0.0f;
    ry.m[2][0] = sy;   ry.m[2][1] = 0.0f; ry.m[2][2] = cy;   ry.m[2][3] = 0.0f;
    ry.m[3][0] = 0.0f; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f; ry.m[3][3] = 1.0f;

    rz.m[0][0] = cz;   rz.m[0][1] = -sz;  rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
    rz.m[1][0] = sz;   rz.m[1][1] = cz;   rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
    rz.m[2][0] = 0.0f; rz.m[2][1] = 0.0f; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
    rz.m[3][0] = 0.0f; rz.m[3][1] = 0.0f; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

    *this = rz * ry * rx;
}

/*void Matrix4f::InitRotateVectorTransform(gs_scalar angle, const Vector3f& vect)
{
    Vector3f V = vect;
    V.Normalize();
    const gs_scalar x = V.x;
    const gs_scalar y = V.y;
    const gs_scalar z = V.z;
    const gs_scalar c = cosd(angle);
    const gs_scalar s = sind(angle);

    m[0][0] = x*x*(1-c)+c;    m[0][1] = x*y*(1-c)-z*s; m[0][2] = x*z*(1-c)+y*s; m[0][3] = 0.0f;
    m[1][0] = y*x*(1-c)+z*s;  m[1][1] = y*y*(1-c)+c;   m[1][2] = y*z*(1-c)-x*s; m[1][3] = 0.0f;
    m[2][0] = x*z*(1-c)-y*s;  m[2][1] = y*z*(1-c)+x*s; m[2][2] = z*z*(1-c)+c;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;           m[3][1] = 0.0f;          m[3][2] = 0.0f;          m[3][3] = 1.0f;
}*/

void Matrix4::InitRotateAxisTransform(gs_scalar angle, gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ)
{
    const gs_scalar x = RotateX;
    const gs_scalar y = RotateY;
    const gs_scalar z = RotateZ;
    const gs_scalar xy = x*y;
    const gs_scalar xz = x*z;
    const gs_scalar yz = y*z;
    const gs_scalar c = cosd(angle);
    const gs_scalar s = sind(angle);

    m[0][0] = x*x*(1-c)+c;    m[0][1] = xy*(1-c)-z*s;  m[0][2] = xz*(1-c)+y*s;  m[0][3] = 0.0f;
    m[1][0] = xy*(1-c)+z*s;   m[1][1] = y*y*(1-c)+c;   m[1][2] = yz*(1-c)-x*s;  m[1][3] = 0.0f;
    m[2][0] = xz*(1-c)-y*s;   m[2][1] = yz*(1-c)+x*s;  m[2][2] = z*z*(1-c)+c;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;           m[3][1] = 0.0f;          m[3][2] = 0.0f;          m[3][3] = 1.0f;
}

void Matrix4::InitRotateXTransform(gs_scalar angle)
{
    const gs_scalar c = cosd(angle);
    const gs_scalar s = sind(angle);

    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = c;    m[1][2] = -s;    m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = s;    m[2][2] = c;    m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4::InitRotateYTransform(gs_scalar angle)
{
    const gs_scalar c = cosd(angle);
    const gs_scalar s = sind(angle);

    m[0][0] = c;    m[0][1] = 0.0f;  m[0][2] = -s;    m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f;  m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = s;    m[2][1] = 0.0f;  m[2][2] = c;    m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f;  m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4::InitRotateZTransform(gs_scalar angle)
{
    const gs_scalar c = cosd(angle);
    const gs_scalar s = sind(angle);

    m[0][0] = c;    m[0][1] = -s;    m[0][2] = 0.0f;  m[0][3] = 0.0f;
    m[1][0] = s;    m[1][1] = c;    m[1][2] = 0.0f;  m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;  m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f;  m[3][3] = 1.0f;
}

void Matrix4::InitTranslationTransform(gs_scalar x, gs_scalar y, gs_scalar z)
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}


void Matrix4::InitCameraTransform(const Vector3& from, const Vector3& to, const Vector3& up)
{
    Vector3 f(to.x - from.x,to.y - from.y,to.z - from.z);
    f.Normalize();

    Vector3 U = up;
    U.Normalize();

    Vector3 s = f.Cross(U);
    s.Normalize();

    U = s.Cross(f);

    m[0][0] = s.x;    m[0][1] = s.y;   m[0][2] = s.z;   m[0][3] = 0.0f;
    m[1][0] = U.x;    m[1][1] = U.y;   m[1][2] = U.z;   m[1][3] = 0.0f;
    m[2][0] = -f.x;   m[2][1] = -f.y;  m[2][2] = -f.z;  m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;

    Matrix4 trans;
    trans.InitTranslationTransform(-from.x,-from.y,-from.z);
    *this = (*this) * trans;
}

void Matrix4::InitPersProjTransform(gs_scalar fovy, gs_scalar aspect_ratio, gs_scalar znear, gs_scalar zfar)
{
    const gs_scalar zRange     = znear - zfar;
    const gs_scalar f = 1.0 / tan(fovy * M_PI / 360.0);

    m[0][0] = f / aspect_ratio; m[0][1] = 0.0f; m[0][2] = 0.0f;                m[0][3] = 0.0f;
    m[1][0] = 0.0f;             m[1][1] = f;    m[1][2] = 0.0f;                m[1][3] = 0.0f;
    m[2][0] = 0.0f;             m[2][1] = 0.0f; m[2][2] = (zfar+znear)/zRange; m[2][3] = 2.0f*zfar*znear/zRange;
    m[3][0] = 0.0f;             m[3][1] = 0.0f; m[3][2] = -1.0f;               m[3][3] = 0.0f;
}

void Matrix4::InitOrthoProjTransform(gs_scalar left, gs_scalar right, gs_scalar bottom, gs_scalar top, gs_scalar znear, gs_scalar zfar)
{
    m[0][0] = 2.0f/(right - left);    m[0][1] = 0.0f;                m[0][2] = 0.0f;                m[0][3] = -(right+left)/(right-left);
    m[1][0] = 0.0f;                   m[1][1] = 2.0f/(top - bottom); m[1][2] = 0.0f;                m[1][3] = -(top+bottom)/(top-bottom);
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;                m[2][2] = -2.0f/(zfar-znear);  m[2][3] = -(zfar+znear)/(zfar-znear);
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;                m[3][2] = 0.0f;                m[3][3] = 1.0f;
}


gs_scalar Matrix4::Determinant() const
{
	return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][1]*m[2][3]*m[3][2] + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][2]*m[2][1]*m[3][3]
		+ m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][2]*m[2][0]*m[3][3]
		- m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][0]*m[2][3]*m[3][2]
		+ m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][3]*m[2][1]*m[3][0] + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][0]*m[2][3]*m[3][1]
		+ m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][1]*m[2][0]*m[3][3] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][0]*m[2][2]*m[3][1]
		- m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
}


Matrix4& Matrix4::Inverse()
{
	// Compute the reciprocal determinant
	gs_scalar det = Determinant();
	if(det == 0.0f) //THIS SHOULD PROBABLY ERROR OUT
	{
	    printf("Cannot do inverse for the 4x4 matrix!\n");
		return *this;
	}

	gs_scalar invdet = 1.0f / det;

	Matrix4 res;
	res.m[0][0] = invdet  * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
	res.m[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
	res.m[0][2] = invdet  * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
	res.m[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
	res.m[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
	res.m[1][1] = invdet  * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
	res.m[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
	res.m[1][3] = invdet  * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
	res.m[2][0] = invdet  * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[2][2] = invdet  * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
	res.m[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
	res.m[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[3][1] = invdet  * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
	res.m[3][3] = invdet  * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
	*this = res;

	return *this;
}

void Quaternion::Normalize()
{
    gs_scalar Length = sqrtf(x * x + y * y + z * z + w * w);

    x /= Length;
    y /= Length;
    z /= Length;
    w /= Length;
}


Quaternion Quaternion::Conjugate()
{
    Quaternion ret(-x, -y, -z, w);
    return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
    const gs_scalar w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
    const gs_scalar x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
    const gs_scalar y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
    const gs_scalar z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

    Quaternion ret(x, y, z, w);

    return ret;
}

Quaternion operator*(const Quaternion& q, const Vector3& v)
{
    const gs_scalar w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    const gs_scalar x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const gs_scalar y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const gs_scalar z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    Quaternion ret(x, y, z, w);

    return ret;
}

}
