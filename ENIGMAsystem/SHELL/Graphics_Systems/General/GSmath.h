/** Copyright (C) 2015 Harijs Grinbergs
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

#ifndef ENIGMA_GSMATH_H
#define	ENIGMA_GSMATH_H

#include "Universal_System/scalar.h"
#include <stdio.h> //For printf
#include <limits> //For numerical limits
//#include "util.h"

namespace enigma
{
  inline bool equal(gs_scalar x, gs_scalar y) { return (::fabs(x-y) <= std::numeric_limits<gs_scalar>::epsilon() * ::fmax(gs_scalar(1.0), ::fmax(::fabs(x), ::fabs(y)))); }

  //Forward declare - This also is useful as a list of classes this file implements
  struct Quaternion;
  struct Vector2;
  struct Vector3;
  struct Vector4;
  struct Matrix3;
  struct Matrix4;

  ////////////////////////////DEFINITIONS//////////////////////////////////

  /////////////////////////
  //Quaternion definition//
  /////////////////////////
  struct Quaternion
  {
    gs_scalar x, y, z, w;

    Quaternion(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar w) : x(x), y(y), z(z), w(w) {}

    Quaternion operator*(const Quaternion& r);
    Quaternion operator*(const Vector3& v);

    void normalize();
    Quaternion conjugate();
  };

  Quaternion operator*(const Quaternion& l, const Quaternion& r);
  Quaternion operator*(const Quaternion& q, const Vector3& v);

  //////////////////////
  //Vector2 definition//
  //////////////////////
  struct Vector2
  {
    gs_scalar x, y;

    Vector2() {};
    Vector2(gs_scalar x, gs_scalar y) : x(x), y(y) {  };
  };

  //////////////////////
  //Vector3 definition//
  //////////////////////
  struct Vector3
  {
    gs_scalar x, y, z;

    Vector3() {};
    Vector3(gs_scalar x, gs_scalar y, gs_scalar z) :  x(x), y(y), z(z) {};

    Vector3& operator+=(const Vector3& r);
    Vector3& operator-=(const Vector3& r);
    Vector3& operator*=(gs_scalar f);

    bool operator== (const Vector3 &V) const;
    bool operator< (const Vector3 &V) const;

    Vector3 cross(const Vector3& v) const;
    gs_scalar dot(const Vector3& v) const;
    Vector3& normalize();
    void rotate(gs_scalar Angle, const Vector3& Axe);
  };

  Vector3 operator+(const Vector3& l, const Vector3& r);
  Vector3 operator-(const Vector3& l, const Vector3& r);
  Vector3 operator*(const Vector3& l, gs_scalar f);

  //////////////////////
  //Vector4 definition//
  //////////////////////
  struct Vector4
  {
    gs_scalar x, y, z, w;

    Vector4() {};
    Vector4(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar w) : x(x), y(y), z(z), w(w) {};

    Vector4& normalize();
  };

  ///////////////////////
  //MATRIX3  definition//
  ///////////////////////
  struct Matrix3
  {
    gs_scalar m[3][3];

    Matrix3() {};
    Matrix3(gs_scalar a00, gs_scalar a01, gs_scalar a02,
            gs_scalar a10, gs_scalar a11, gs_scalar a12,
            gs_scalar a20, gs_scalar a21, gs_scalar a22){
      m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
      m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
      m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
    }

    operator gs_scalar*();
    Vector3 operator*(const Vector3& v) const;
    Matrix3 operator*(const Matrix3& Right) const;
    gs_scalar operator() (unsigned int a, unsigned int b) const;

    void set_zero();
    Matrix3 transpose() const;
    void init_identity();
    gs_scalar determinant() const;
    Matrix3& inverse();
    void print() const;
  };


  ///////////////////////
  //MATRIX4  definition//
  ///////////////////////
  struct Matrix4
  {
    gs_scalar m[4][4];

    Matrix4() { };

    Matrix4(gs_scalar a00, gs_scalar a01, gs_scalar a02, gs_scalar a03,
            gs_scalar a10, gs_scalar a11, gs_scalar a12, gs_scalar a13,
            gs_scalar a20, gs_scalar a21, gs_scalar a22, gs_scalar a23,
            gs_scalar a30, gs_scalar a31, gs_scalar a32, gs_scalar a33) {
      m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
      m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
      m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
      m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }

    Matrix4(const gs_scalar *array) {
      gs_scalar *i = (gs_scalar*)m;
      const gs_scalar *j =  array;
      for (int k=0; k<16; i++,j++,k++)
        *i = *j;
    }

    inline void set(gs_scalar a00, gs_scalar a01, gs_scalar a02, gs_scalar a03,
                    gs_scalar a10, gs_scalar a11, gs_scalar a12, gs_scalar a13,
                    gs_scalar a20, gs_scalar a21, gs_scalar a22, gs_scalar a23,
                    gs_scalar a30, gs_scalar a31, gs_scalar a32, gs_scalar a33) {
      m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
      m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
      m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
      m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }

    /*Matrix4f operator*(const Matrix4f& n) const;*/
    Matrix4 operator*(const Matrix4& Right) const;
    Vector4 operator*(const Vector4& v) const;
    gs_scalar operator() (unsigned int a, unsigned int b) const;
    operator gs_scalar*();

    void print() const;
    gs_scalar determinant() const;
    Matrix4& inverse();
    void set_zero();
    Matrix4 transpose() const;
    void init_identity();

    void scale(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ);
    void rotate(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    void rotate(gs_scalar angle, gs_scalar x, gs_scalar y, gs_scalar z);
    void rotate_x(gs_scalar angle);
    void rotate_y(gs_scalar angle);
    void rotate_z(gs_scalar angle);
    void translate(gs_scalar x, gs_scalar y, gs_scalar z);

    void init_scale_transform(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ);
    void init_rotate_transform(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    void init_rotate_axis_transform(gs_scalar angle, gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ);
    void init_rotate_x_transform(gs_scalar angle);
    void init_rotate_y_transform(gs_scalar angle);
    void init_rotate_z_transform(gs_scalar angle);
    void init_translation_transform(gs_scalar x, gs_scalar y, gs_scalar z);
    void init_look_at_transform(const Vector3& from, const Vector3& to, const Vector3& up);

    void init_camera_transform(const Vector3& from, const Vector3& to, const Vector3& up);
    void init_perspective_proj_transform(gs_scalar fovy, gs_scalar aspect_ratio, gs_scalar znear, gs_scalar zfar);
    void init_ortho_proj_transform(gs_scalar left, gs_scalar right, gs_scalar bottom, gs_scalar top, gs_scalar znear, gs_scalar zfar);
  };

  ////////////////////////////IMPLEMENTATIONS//////////////////////////////////

  /////////////////////////////
  //Quaternion implementation//
  /////////////////////////////
  inline void Quaternion::normalize(){
    const gs_scalar l = sqrtf(x * x + y * y + z * z + w * w);

    x /= l;
    y /= l;
    z /= l;
    w /= l;
  }

  inline Quaternion Quaternion::conjugate(){
    return Quaternion(-x, -y, -z, w);
  }

  inline Quaternion Quaternion::operator*(const Quaternion& r){
    const gs_scalar wn = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
    const gs_scalar xn = (x * r.w) + (w * r.x) + (y * r.z) - (z * r.y);
    const gs_scalar yn = (y * r.w) + (w * r.y) + (z * r.x) - (x * r.z);
    const gs_scalar zn = (z * r.w) + (w * r.z) + (x * r.y) - (y * r.x);

    return Quaternion(xn, yn, zn, wn);
  }

  inline Quaternion Quaternion::operator*(const Vector3& v){
    const gs_scalar wn = - (x * v.x) - (y * v.y) - (z * v.z);
    const gs_scalar xn =   (w * v.x) + (y * v.z) - (z * v.y);
    const gs_scalar yn =   (w * v.y) + (z * v.x) - (x * v.z);
    const gs_scalar zn =   (w * v.z) + (x * v.y) - (y * v.x);

    return Quaternion(xn, yn, zn, wn);
  }

  inline Quaternion operator*(const Quaternion& l, const Quaternion& r){ return l*r; }
  inline Quaternion operator*(const Quaternion& q, const Vector3& v){ return q*v; }


  //////////////////////////
  //Vector3 implementation//
  //////////////////////////
  inline Vector3& Vector3::operator+=(const Vector3& r){
      x += r.x;
      y += r.y;
      z += r.z;

      return *this;
  }

  inline Vector3& Vector3::operator-=(const Vector3& r){
      x -= r.x;
      y -= r.y;
      z -= r.z;

      return *this;
  }

  inline Vector3& Vector3::operator*=(gs_scalar f){
      x *= f;
      y *= f;
      z *= f;

      return *this;
  }

  inline bool Vector3::operator== (const Vector3 &V) const{
    return (equal(x,V.x) && equal(y,V.y) && equal(z,V.z));
  }

  inline bool Vector3::operator< (const Vector3 &V) const{
    if (x != V.x) return x < V.x;
    if (y != V.y) return y < V.y;
    if (z != V.z) return z < V.z;
    return false;
  }

  inline Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(y * v.z - z * v.y,
                   z * v.x - x * v.z,
                   x * v.y - y * v.x);
  }

  inline gs_scalar Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
  }

  inline Vector3& Vector3::normalize(){
    const gs_scalar l = sqrt(x * x + y * y + z * z);

    x /= l;
    y /= l;
    z /= l;

    return *this;
  }

  inline void Vector3::rotate(gs_scalar Angle, const Vector3& Axe){
    const gs_scalar SinHalfAngle = sin(Angle/2.0);
    const gs_scalar CosHalfAngle = cos(Angle/2.0);

    const gs_scalar Rx = Axe.x * SinHalfAngle;
    const gs_scalar Ry = Axe.y * SinHalfAngle;
    const gs_scalar Rz = Axe.z * SinHalfAngle;
    const gs_scalar Rw = CosHalfAngle;
    Quaternion RotationQ(Rx, Ry, Rz, Rw);

    Quaternion ConjugateQ = RotationQ.conjugate();
    //  ConjugateQ.Normalize();
    Quaternion W = RotationQ * (*this) * ConjugateQ;

    x = W.x;
    y = W.y;
    z = W.z;
  }

  inline Vector3 operator+(const Vector3& l, const Vector3& r){
    return Vector3(l.x + r.x,
                   l.y + r.y,
                   l.z + r.z);
  }

  inline Vector3 operator-(const Vector3& l, const Vector3& r){
    return Vector3(l.x - r.x,
                   l.y - r.y,
                   l.z - r.z);
  }

  inline Vector3 operator*(const Vector3& l, gs_scalar f){
    return Vector3(l.x * f,
                   l.y * f,
                   l.z * f);
  }

  //////////////////////////
  //Vector4 implementation//
  //////////////////////////
  inline Vector4& Vector4::normalize(){
    const gs_scalar l = sqrt(x * x + y * y + z * z + w * w);

    x /= l;
    y /= l;
    z /= l;
    w /= l;

    return *this;
  }

  //////////////////////////
  //Matrix3 implementation//
  //////////////////////////
  inline void Matrix3::set_zero(){
    m[0][0] = 0.0; m[0][1] = 0.0; m[0][2] = 0.0;
    m[1][0] = 0.0; m[1][1] = 0.0; m[1][2] = 0.0;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 0.0;
  }

  inline Matrix3 Matrix3::transpose() const {
    return Matrix3(m[0][0], m[1][0], m[2][0],
                   m[0][1], m[1][1], m[2][1],
                   m[0][2], m[1][2], m[2][2]);
  }

  inline void Matrix3::init_identity() {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
  }

  inline gs_scalar Matrix3::operator() (unsigned int a, unsigned int b) const {
    return m[a][b];
  }

  inline Matrix3::operator gs_scalar*() {
    return &(m[0][0]);
  }

  inline Vector3 Matrix3::operator*(const Vector3& v) const {
    return Vector3(m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z, //x
                   m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z, //y
                   m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z); //z
  }

  inline Matrix3 Matrix3::operator*(const Matrix3& Right) const {
    return Matrix3(m[0][0] * Right.m[0][0] + m[0][1] * Right.m[1][0] + m[0][2] * Right.m[2][0], //m[0][0]
                   m[0][0] * Right.m[0][1] + m[0][1] * Right.m[1][1] + m[0][2] * Right.m[2][1], //m[0][1]
                   m[0][0] * Right.m[0][2] + m[0][2] * Right.m[1][2] + m[0][2] * Right.m[2][2], //m[0][2]
                   m[1][0] * Right.m[0][0] + m[1][2] * Right.m[1][0] + m[1][2] * Right.m[2][0], //m[1][0]
                   m[1][0] * Right.m[0][1] + m[1][2] * Right.m[1][1] + m[1][2] * Right.m[2][1], //m[1][1]
                   m[1][0] * Right.m[0][2] + m[1][2] * Right.m[1][2] + m[1][2] * Right.m[2][2], //m[1][2]
                   m[2][0] * Right.m[0][0] + m[2][2] * Right.m[1][0] + m[2][2] * Right.m[2][0], //m[2][0]
                   m[2][0] * Right.m[0][1] + m[2][2] * Right.m[1][1] + m[2][2] * Right.m[2][1], //m[2][1]
                   m[2][0] * Right.m[0][2] + m[2][2] * Right.m[1][2] + m[2][2] * Right.m[2][2]); //m[2][2]
  }

  inline void Matrix3::print() const {
    printf("%f %f %f\n", m[0][0], m[0][1], m[0][2]);
    printf("%f %f %f\n", m[1][0], m[1][1], m[1][2]);
    printf("%f %f %f\n", m[2][0], m[2][1], m[2][2]);
  }

  inline gs_scalar Matrix3::determinant() const {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  }

  inline Matrix3& Matrix3::inverse() {
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
    const gs_scalar determinant = m[0][0] * tmp[0] + m[0][1] * tmp[3] + m[0][2] * tmp[6];
    if(fabs(determinant) <= 0.00001f)  //THIS SHOULD PROBABLY ERROR OUT
    {
        printf("Cannot do inverse for the 3x3 matrix!\n");
        return *this;
    }

    // divide by the determinant
    const gs_scalar invDeterminant = 1.0f / determinant;
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


  ///////////////////////////
  //MATRIX4  implementation//
  ///////////////////////////
  inline void Matrix4::set_zero() {
    m[0][0] = 0.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
    m[1][0] = 0.0; m[1][1] = 0.0; m[1][2] = 0.0; m[1][3] = 0.0;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 0.0; m[2][3] = 0.0;
    m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 0.0;
  }

  inline Matrix4 Matrix4::transpose() const {
    return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                   m[0][1], m[1][1], m[2][1], m[3][1],
                   m[0][2], m[1][2], m[2][2], m[3][2],
                   m[0][3], m[1][3], m[2][3], m[3][3]);
  }


  inline void Matrix4::init_identity() {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  inline Matrix4 Matrix4::operator*(const Matrix4& Right) const {
    return Matrix4(m[0][0] * Right.m[0][0] + m[0][1] * Right.m[1][0] + m[0][2] * Right.m[2][0] + m[0][3] * Right.m[3][0],
                   m[0][0] * Right.m[0][1] + m[0][1] * Right.m[1][1] + m[0][2] * Right.m[2][1] + m[0][3] * Right.m[3][1],
                   m[0][0] * Right.m[0][2] + m[0][1] * Right.m[1][2] + m[0][2] * Right.m[2][2] + m[0][3] * Right.m[3][2],
                   m[0][0] * Right.m[0][3] + m[0][1] * Right.m[1][3] + m[0][2] * Right.m[2][3] + m[0][3] * Right.m[3][3],
                   m[1][0] * Right.m[0][0] + m[1][1] * Right.m[1][0] + m[1][2] * Right.m[2][0] + m[1][3] * Right.m[3][0],
                   m[1][0] * Right.m[0][1] + m[1][1] * Right.m[1][1] + m[1][2] * Right.m[2][1] + m[1][3] * Right.m[3][1],
                   m[1][0] * Right.m[0][2] + m[1][1] * Right.m[1][2] + m[1][2] * Right.m[2][2] + m[1][3] * Right.m[3][2],
                   m[1][0] * Right.m[0][3] + m[1][1] * Right.m[1][3] + m[1][2] * Right.m[2][3] + m[1][3] * Right.m[3][3],
                   m[2][0] * Right.m[0][0] + m[2][1] * Right.m[1][0] + m[2][2] * Right.m[2][0] + m[2][3] * Right.m[3][0],
                   m[2][0] * Right.m[0][1] + m[2][1] * Right.m[1][1] + m[2][2] * Right.m[2][1] + m[2][3] * Right.m[3][1],
                   m[2][0] * Right.m[0][2] + m[2][1] * Right.m[1][2] + m[2][2] * Right.m[2][2] + m[2][3] * Right.m[3][2],
                   m[2][0] * Right.m[0][3] + m[2][1] * Right.m[1][3] + m[2][2] * Right.m[2][3] + m[2][3] * Right.m[3][3],
                   m[3][0] * Right.m[0][0] + m[3][1] * Right.m[1][0] + m[3][2] * Right.m[2][0] + m[3][3] * Right.m[3][0],
                   m[3][0] * Right.m[0][1] + m[3][1] * Right.m[1][1] + m[3][2] * Right.m[2][1] + m[3][3] * Right.m[3][1],
                   m[3][0] * Right.m[0][2] + m[3][1] * Right.m[1][2] + m[3][2] * Right.m[2][2] + m[3][3] * Right.m[3][2],
                   m[3][0] * Right.m[0][3] + m[3][1] * Right.m[1][3] + m[3][2] * Right.m[2][3] + m[3][3] * Right.m[3][3]);
  }

  inline Vector4 Matrix4::operator*(const Vector4& v) const {
    return Vector4(m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z + m[0][3]* v.w,
                   m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z + m[1][3]* v.w,
                   m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z + m[2][3]* v.w,
                   m[3][0]* v.x + m[3][1]* v.y + m[3][2]* v.z + m[3][3]* v.w);
  }

  inline gs_scalar Matrix4::operator() (unsigned int a, unsigned int b) const{
    return m[a][b];
  }

  inline Matrix4::operator gs_scalar*() {
    return &(m[0][0]);
  }

  inline void Matrix4::print() const {
    printf("%f %f %f %f\n", m[0][0], m[0][1], m[0][2], m[0][3]);
    printf("%f %f %f %f\n", m[1][0], m[1][1], m[1][2], m[1][3]);
    printf("%f %f %f %f\n", m[2][0], m[2][1], m[2][2], m[2][3]);
    printf("%f %f %f %f\n", m[3][0], m[3][1], m[3][2], m[3][3]);
  }

  inline gs_scalar Matrix4::determinant() const {
    return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][1]*m[2][3]*m[3][2] + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][2]*m[2][1]*m[3][3]
         + m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][2]*m[2][0]*m[3][3]
         - m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][0]*m[2][3]*m[3][2]
         + m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][3]*m[2][1]*m[3][0] + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][0]*m[2][3]*m[3][1]
         + m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][1]*m[2][0]*m[3][3] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][0]*m[2][2]*m[3][1]
         - m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
  }

  inline void Matrix4::translate(gs_scalar x, gs_scalar y, gs_scalar z){
    m[0][0] += m[3][0]*x; m[0][1] += m[3][1]*x; m[0][2] += m[3][2]*x; m[0][3] += m[3][3]*x;
    m[1][0] += m[3][0]*y; m[1][1] += m[3][1]*y; m[1][2] += m[3][2]*y; m[1][3] += m[3][3]*y;
    m[2][0] += m[3][0]*z; m[2][1] += m[3][1]*z; m[2][2] += m[3][2]*z; m[2][3] += m[3][3]*z;
  }

  inline void Matrix4::scale(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ) {
    m[0][0] *= ScaleX;  m[0][1] *= ScaleX;  m[0][2] *= ScaleX;  m[0][3] *= ScaleX;
    m[1][0] *= ScaleY;  m[1][1] *= ScaleY;  m[1][2] *= ScaleY;  m[1][3] *= ScaleY;
    m[2][0] *= ScaleZ;  m[2][1] *= ScaleZ;  m[2][2] *= ScaleZ;  m[2][3] *= ScaleZ;
  }

  inline void Matrix4::rotate(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ){
    Matrix4 sm;
    sm.init_rotate_transform(RotateX, RotateY, RotateZ);
    *this = sm*(*this);
  }

  inline void Matrix4::rotate(gs_scalar angle, gs_scalar x, gs_scalar y, gs_scalar z){
    Matrix4 sm;
    sm.init_rotate_axis_transform(angle, x, y, z);
    *this = sm*(*this);
  }

  inline void Matrix4::rotate_x(gs_scalar angle){
    const gs_scalar s = sin(angle);
    const gs_scalar c = cos(angle);
    const gs_scalar m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3],
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

  inline void Matrix4::rotate_y(gs_scalar angle){
    const gs_scalar s = sin(angle);
    const gs_scalar c = cos(angle);
    const gs_scalar m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3],
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

  inline void Matrix4::rotate_z(gs_scalar angle){
    const gs_scalar s = sin(angle);
    const gs_scalar c = cos(angle);
    const gs_scalar m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3],
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

  inline Matrix4& Matrix4::inverse(){
    // Compute the reciprocal determinant
    double det = determinant();
    if(det == 0.0f) //THIS SHOULD PROBABLY ERROR OUT
    {
      printf("Cannot do inverse for the 4x4 matrix!\n");
      return *this;
    }

    double invdet = 1.0f / det;

    set(invdet  * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1])), //m[0][0]
        -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1])), //m[0][1]
        invdet  * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1])), //m[0][2]
        -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])), //m[0][3]
        -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0])), //m[1][0]
        invdet  * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0])), //m[1][1]
        -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0])), //m[1][2]
        invdet  * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])), //m[1][3]
        invdet  * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0])), //m[2][0]
        -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0])), //m[2][1]
        invdet  * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0])), //m[2][2]
        -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0])), //m[2][3]
        -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0])), //m[3][0]
        invdet  * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0])), //m[3][1]
        -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0])), //m[3][2]
        invdet  * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]))); //m[3][3]

    return *this;
  }

  inline void Matrix4::init_scale_transform(gs_scalar ScaleX, gs_scalar ScaleY, gs_scalar ScaleZ){
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
  }

  inline void Matrix4::init_rotate_transform(gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ){
    Matrix4 rx, ry, rz;
    const gs_scalar x = RotateX;
    const gs_scalar y = RotateY;
    const gs_scalar z = RotateZ;

    const gs_scalar sx = sin(x);
    const gs_scalar cx = cos(x);

    const gs_scalar sy = sin(y);
    const gs_scalar cy = cos(y);

    const gs_scalar sz = sin(z);
    const gs_scalar cz = cos(z);

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

  inline void Matrix4::init_rotate_axis_transform(gs_scalar angle, gs_scalar RotateX, gs_scalar RotateY, gs_scalar RotateZ){
    const gs_scalar x = RotateX;
    const gs_scalar y = RotateY;
    const gs_scalar z = RotateZ;
    const gs_scalar xy = x*y;
    const gs_scalar xz = x*z;
    const gs_scalar yz = y*z;
    const gs_scalar s = sin(angle);
    const gs_scalar c = cos(angle);

    m[0][0] = x*x*(1-c)+c;    m[0][1] = xy*(1-c)-z*s;  m[0][2] = xz*(1-c)+y*s;  m[0][3] = 0.0f;
    m[1][0] = xy*(1-c)+z*s;   m[1][1] = y*y*(1-c)+c;   m[1][2] = yz*(1-c)-x*s;  m[1][3] = 0.0f;
    m[2][0] = xz*(1-c)-y*s;   m[2][1] = yz*(1-c)+x*s;  m[2][2] = z*z*(1-c)+c;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;           m[3][1] = 0.0f;          m[3][2] = 0.0f;          m[3][3] = 1.0f;
  }

  inline void Matrix4::init_rotate_x_transform(gs_scalar angle){
    const gs_scalar s = sin(angle);
    const gs_scalar c = cos(angle);

    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = c;    m[1][2] = -s;    m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = s;    m[2][2] = c;    m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  inline void Matrix4::init_rotate_y_transform(gs_scalar angle){
    const gs_scalar s = sin(angle);
    const gs_scalar c = cos(angle);

    m[0][0] = c;    m[0][1] = 0.0f;  m[0][2] = -s;    m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f;  m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = s;    m[2][1] = 0.0f;  m[2][2] = c;    m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f;  m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  inline void Matrix4::init_rotate_z_transform(gs_scalar angle){
    const gs_scalar s = sin(angle);
    const gs_scalar c = cos(angle);

    m[0][0] = c;    m[0][1] = -s;    m[0][2] = 0.0f;  m[0][3] = 0.0f;
    m[1][0] = s;    m[1][1] = c;    m[1][2] = 0.0f;  m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;  m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f;  m[3][3] = 1.0f;
  }

  inline void Matrix4::init_translation_transform(gs_scalar x, gs_scalar y, gs_scalar z){
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  inline void Matrix4::init_camera_transform(const Vector3& from, const Vector3& to, const Vector3& up){
    Vector3 zaxis = (to-from).normalize();
    Vector3 xaxis = zaxis.cross(up).normalize();
    Vector3 yaxis = xaxis.cross(zaxis);
            
    m[0][0] = xaxis.x;    m[0][1] = xaxis.y;   m[0][2] = xaxis.z;   m[0][3] = -xaxis.dot(from);
    m[1][0] = yaxis.x;    m[1][1] = yaxis.y;   m[1][2] = yaxis.z;   m[1][3] = -yaxis.dot(from);
    m[2][0] =-zaxis.x;    m[2][1] =-zaxis.y;   m[2][2] =-zaxis.z;   m[2][3] = zaxis.dot(from);
    m[3][0] = 0.0f;       m[3][1] = 0.0f;      m[3][2] = 0.0f;      m[3][3] = 1.0f;
  }

  inline void Matrix4::init_perspective_proj_transform(gs_scalar fovy, gs_scalar aspect_ratio, gs_scalar znear, gs_scalar zfar){
    const gs_scalar zRange     = znear - zfar;
    const gs_scalar f = 1.0 / tan(fovy * M_PI / 360.0);

    m[0][0] = f / aspect_ratio; m[0][1] = 0.0f; m[0][2] = 0.0f;                m[0][3] = 0.0f;
    m[1][0] = 0.0f;             m[1][1] = f;    m[1][2] = 0.0f;                m[1][3] = 0.0f;
    m[2][0] = 0.0f;             m[2][1] = 0.0f; m[2][2] = (zfar+znear)/zRange; m[2][3] = 2.0f*zfar*znear/zRange;
    m[3][0] = 0.0f;             m[3][1] = 0.0f; m[3][2] = -1.0f;               m[3][3] = 0.0f;
  }

  inline void Matrix4::init_ortho_proj_transform(gs_scalar left, gs_scalar right, gs_scalar bottom, gs_scalar top, gs_scalar znear, gs_scalar zfar){
    m[0][0] = 2.0f/(right - left);    m[0][1] = 0.0f;                m[0][2] = 0.0f;                m[0][3] = -(right+left)/(right-left);
    m[1][0] = 0.0f;                   m[1][1] = 2.0f/(top - bottom); m[1][2] = 0.0f;                m[1][3] = -(top+bottom)/(top-bottom);
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;                m[2][2] = -2.0f/(zfar-znear);  m[2][3] = -(zfar+znear)/(zfar-znear);
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;                m[3][2] = 0.0f;                m[3][3] = 1.0f;
  }

  inline void Matrix4::init_look_at_transform(const Vector3& from, const Vector3& to, const Vector3& wup){

    Vector3 up = wup;
    Vector3 direction = (to-from).normalize();

    up.normalize();
    Vector3 right = up.cross(direction).normalize();
    up = direction.cross(right).normalize();

    m[0][0] = right.x;    m[0][1] = up.x;   m[0][2] = direction.x;   m[0][3] = from.x;
    m[1][0] = right.y;    m[1][1] = up.y;   m[1][2] = direction.y;   m[1][3] = from.y;
    m[2][0] = right.z;    m[2][1] = up.z;   m[2][2] = direction.z;   m[2][3] = from.z;
    m[3][0] = 0.0f;       m[3][1] = 0.0f;   m[3][2] = 0.0f;          m[3][3] = 1.0f;
  }

}
#endif

