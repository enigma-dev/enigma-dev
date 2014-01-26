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
#include <algorithm>  //For std::copy - could probably be done better
#include "GSmath.h"

namespace enigma
{

#define cosd(x)  ::cos(x * M_PI / 180.0)
#define sind(x)  ::sin(x * M_PI / 180.0)
#define tand(x)  ::tan(x * M_PI / 180.0)
#define degtorad(x) x*(M_PI/180.0)

Vector3f Vector3f::Cross(const Vector3f& v) const
{
    const gs_scalar _x = y * v.z - z * v.y;
    const gs_scalar _y = z * v.x - x * v.z;
    const gs_scalar _z = x * v.y - y * v.x;

    return Vector3f(_x, _y, _z);
}

Vector3f& Vector3f::Normalize()
{
    const gs_scalar Length = sqrtf(x * x + y * y + z * z);

    x /= Length;
    y /= Length;
    z /= Length;

    return *this;
}

void Vector3f::Rotate(gs_scalar Angle, const Vector3f& Axe)
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

//This 3 could techincally be expanded like others - It would of be faster
void Matrix4f::scale(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ)
{
    Matrix4f sm, tm;
    sm.InitScaleTransform(ScaleX, ScaleY, ScaleZ);
    std::copy(m, m+16, tm.m);
    *this = tm*sm;
}

//By a vector
void Matrix4f::rotate(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ)
{
    Matrix4f sm, tm;
    sm.InitRotateTransform(RotateX, RotateY, RotateZ);
    std::copy(m, m+16, tm.m);
    *this = tm*sm;
}

//Around an axis
void Matrix4f::rotate(gs_scalar angle, gs_scalar x, gs_scalar y, gs_scalar z)
{
    Matrix4f sm, tm;
    Vector3f V(x,y,z);
    sm.InitRotateVectorTransform(angle, V);
    std::copy(m, m+16, tm.m);
    *this = tm*sm;
}

void Matrix4f::translate(gs_scalar x, gs_scalar y, gs_scalar z)
{
    Matrix4f sm, tm;
    sm.InitTranslationTransform(x, y, z);
    std::copy(m, m+16, tm.m);
    *this = tm*sm;
}

void Matrix4f::InitScaleTransform(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ)
{
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ)
{
    Matrix4f rx, ry, rz;
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

void Matrix4f::InitRotateVectorTransform(gs_scalar angle, const Vector3f& vect)
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
    m[2][0] = x*z*(1-c)-y*s; m[2][1] = y*z*(1-c)+x*s; m[2][2] = z*z*(1-c)+c;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;          m[3][1] = 0.0f;          m[3][2] = 0.0f;          m[3][3] = 1.0f;
}

void Matrix4f::InitTranslationTransform(gs_scalar x, gs_scalar y, gs_scalar z)
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}


void Matrix4f::InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
{
    Vector3f N = Target;
    N.Normalize();
    Vector3f U = Up;
    U = U.Cross(N);
    U.Normalize();
    Vector3f V = N.Cross(U);
    V.Normalize();

    m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
    m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
    m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
}

void Matrix4f::InitPersProjTransform(gs_scalar fovy, gs_scalar aspect_ratio, gs_scalar znear, gs_scalar zfar)
{
    const gs_scalar ar         = aspect_ratio;
    const gs_scalar zRange     = znear - zfar;
    const gs_scalar tanHalfFOV = tand(fovy / 2.0);

    m[0][0] = 1.0f/(tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;            m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f/tanHalfFOV; m[1][2] = 0.0f;            m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-znear - zfar)/zRange ; m[2][3] = 2.0f*zfar*znear/zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;            m[3][3] = 0.0;
}

void Matrix4f::InitOtrhoProjTransform(gs_scalar left, gs_scalar right, gs_scalar bottom, gs_scalar top, gs_scalar znear, gs_scalar zfar)
{
    m[0][0] = 2.0f/(right - left);    m[0][1] = 0.0f;                m[0][2] = 0.0f;                m[0][3] = -(right+left)/(right-left);
    m[1][0] = 0.0f;                   m[1][1] = 2.0f/(top - bottom); m[1][2] = 0.0f;                m[1][3] = -(top+bottom)/(top-bottom);
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;                m[2][2] = -2.0f/(zfar-znear);  m[2][3] = -(zfar+znear)/(zfar-znear);
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;                m[3][2] = 1.0f;                m[3][3] = 0.0;
}


gs_scalar Matrix4f::Determinant() const
{
	return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][1]*m[2][3]*m[3][2] + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][2]*m[2][1]*m[3][3]
		+ m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][2]*m[2][0]*m[3][3]
		- m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][0]*m[2][3]*m[3][2]
		+ m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][3]*m[2][1]*m[3][0] + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][0]*m[2][3]*m[3][1]
		+ m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][1]*m[2][0]*m[3][3] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][0]*m[2][2]*m[3][1]
		- m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
}


Matrix4f& Matrix4f::Inverse()
{
	// Compute the reciprocal determinant
	gs_scalar det = Determinant();
	if(det == 0.0f) //THIS SHOULD PROBABLY ERROR OUT
	{
		return *this;
	}

	gs_scalar invdet = 1.0f / det;

	Matrix4f res;
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

Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
    const gs_scalar w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    const gs_scalar x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const gs_scalar y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const gs_scalar z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    Quaternion ret(x, y, z, w);

    return ret;
}

}
