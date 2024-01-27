#pragma once

#ifdef _WIN64
#ifndef _USE_MATH_DEFINES
// For some reason this works on in Debug build of Visual Studio but not in
// Release build. The solution people suggested was to define this as a
// preprocessor macro in the project.
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#else
#include <math.h>
#endif
#include <cfloat>
#include <stdio.h>

#include <ogldev/utility.h>

#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>

// #define USE_GLM

#ifdef USE_GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#endif
#include <iostream>
#include <stdlib.h>

#include <ogldev/utility.h>

// powf wrapper for integer params to avoid crazy casting
#define powi(base, exp) (int)powf((float)(base), (float)(exp))

#define ToRadian(x) (float)(((x)*M_PI / 180.0f))
#define ToDegree(x) (float)(((x)*180.0f / M_PI))

float RandomFloat();
float RandomFloatRange(float Start, float End);

struct Vector2i {
  int x;
  int y;
};

struct Vector3i {
  union {
    float x;
    float r;
  };

  union {
    float y;
    float g;
  };

  union {
    float z;
    float b;
  };
};

struct Vector2f {
  union {
    float x;
    float u;
  };

  union {
    float y;
    float v;
  };

  Vector2f() {}

  Vector2f(float _x, float _y) {
    x = _x;
    y = _y;
  }

  void Print(bool endl = true) const {
    printf("(%f, %f)", x, y);

    if (endl) {
      printf("\n");
    }
  }

  float Length() const {
    float len = sqrtf(x * x + y * y);
    return len;
  }

  void Normalize() {
    float len = Length();

    assert(len != 0);

    x /= len;
    y /= len;
  }
};

inline Vector2f operator*(const Vector2f &l, float f) {
  Vector2f Ret(l.x * f, l.y * f);

  return Ret;
}

struct Vector4f;

struct Vector3f {
  union {
    float x;
    float r;
  };

  union {
    float y;
    float g;
  };

  union {
    float z;
    float b;
  };

  Vector3f() {}

  Vector3f(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  Vector3f(const float *pFloat) {
    x = pFloat[0];
    y = pFloat[1];
    z = pFloat[2];
  }

  void InitBySphericalCoords(float Radius, float Pitch, float Heading) {
    x = Radius * cosf(ToRadian(Pitch)) * sinf(ToRadian(Heading));
    y = -Radius * sinf(ToRadian(Pitch));
    z = Radius * cosf(ToRadian(Pitch)) * cosf(ToRadian(Heading));
  }

  Vector3f(float f) { x = y = z = f; }

  Vector3f(const Vector4f &v);

  Vector3f &operator+=(const Vector3f &r) {
    x += r.x;
    y += r.y;
    z += r.z;

    return *this;
  }

  Vector3f &operator-=(const Vector3f &r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;

    return *this;
  }

  Vector3f &operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;

    return *this;
  }

  bool operator==(const Vector3f &r) {
    return ((x == r.x) && (y == r.y) && (z == r.z));
  }

  bool operator!=(const Vector3f &r) { return !(*this == r); }

  operator const float *() const { return &(x); }

  Vector3f Cross(const Vector3f &v) const;

  float Dot(const Vector3f &v) const {
    float ret = x * v.x + y * v.y + z * v.z;
    return ret;
  }

  float Distance(const Vector3f &v) const {
    float delta_x = x - v.x;
    float delta_y = y - v.y;
    float delta_z = z - v.z;
    float distance =
        sqrtf(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
    return distance;
  }

  float Length() const {
    float len = sqrtf(x * x + y * y + z * z);
    return len;
  }

  bool IsZero() const { return ((x + y + z) == 0.0f); }

  Vector3f &Normalize();

  void Rotate(float Angle, const Vector3f &Axis);

  Vector3f Negate() const;

  void Print(bool endl = true) const {
    printf("(%f, %f, %f)", x, y, z);

    if (endl) {
      printf("\n");
    }
  }
};

struct Vector4f {
  float x;
  float y;
  float z;
  float w;

  Vector4f() {}

  Vector4f(float _x, float _y, float _z, float _w) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }

  Vector4f(const Vector3f &v, float _w) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = _w;
  }

  void Print(bool endl = true) const {
    printf("(%f, %f, %f, %f)", x, y, z, w);

    if (endl) {
      printf("\n");
    }
  }

  Vector3f to3f() const {
    Vector3f v(x, y, z);
    return v;
  }

  float Length() const {
    float len = sqrtf(x * x + y * y + z * z + w * w);
    return len;
  }

  Vector4f &Normalize();

  float Dot(const Vector4f &v) const {
    float ret = x * v.x + y * v.y + z * v.z + w * v.w;
    return ret;
  }
};

inline Vector3f operator+(const Vector3f &l, const Vector3f &r) {
  Vector3f Ret(l.x + r.x, l.y + r.y, l.z + r.z);

  return Ret;
}

inline Vector3f operator-(const Vector3f &l, const Vector3f &r) {
  Vector3f Ret(l.x - r.x, l.y - r.y, l.z - r.z);

  return Ret;
}

inline Vector3f operator*(const Vector3f &l, float f) {
  Vector3f Ret(l.x * f, l.y * f, l.z * f);

  return Ret;
}

inline Vector3f operator/(const Vector3f &l, float f) {
  Vector3f Ret(l.x / f, l.y / f, l.z / f);

  return Ret;
}

inline Vector3f::Vector3f(const Vector4f &v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

inline Vector4f operator+(const Vector4f &l, const Vector4f &r) {
  Vector4f Ret(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w);

  return Ret;
}

inline Vector4f operator-(const Vector4f &l, const Vector4f &r) {
  Vector4f Ret(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w);

  return Ret;
}

inline Vector4f operator/(const Vector4f &l, float f) {
  Vector4f Ret(l.x / f, l.y / f, l.z / f, l.w / f);

  return Ret;
}

inline Vector4f operator*(const Vector4f &l, float f) {
  Vector4f Ret(l.x * f, l.y * f, l.z * f, l.w * f);

  return Ret;
}

inline Vector4f operator*(float f, const Vector4f &l) {
  Vector4f Ret(l.x * f, l.y * f, l.z * f, l.w * f);

  return Ret;
}

struct PersProjInfo {
  float FOV;
  float Width;
  float Height;
  float zNear;
  float zFar;
};

struct OrthoProjInfo {
  float r; // right
  float l; // left
  float b; // bottom
  float t; // top
  float n; // z near
  float f; // z far

  float Width;
  float Height;

  void Print() {
    printf("Left %f   Right %f\n", l, r);
    printf("Bottom %f Top %f\n", b, t);
    printf("Near %f   Far %f\n", n, f);
  }
};

struct Quaternion {
  float x, y, z, w;

  Quaternion(float Angle, const Vector3f &V);

  Quaternion(float _x, float _y, float _z, float _w);

  void Normalize();

  Quaternion Conjugate() const;

  Vector3f ToDegrees();
};

Quaternion operator*(const Quaternion &l, const Quaternion &r);

Quaternion operator*(const Quaternion &q, const Vector3f &v);

class Matrix4f {
public:
  float m[4][4];

  Matrix4f() {}

  Matrix4f(float a00, float a01, float a02, float a03, float a10, float a11,
           float a12, float a13, float a20, float a21, float a22, float a23,
           float a30, float a31, float a32, float a33) {
    m[0][0] = a00;
    m[0][1] = a01;
    m[0][2] = a02;
    m[0][3] = a03;
    m[1][0] = a10;
    m[1][1] = a11;
    m[1][2] = a12;
    m[1][3] = a13;
    m[2][0] = a20;
    m[2][1] = a21;
    m[2][2] = a22;
    m[2][3] = a23;
    m[3][0] = a30;
    m[3][1] = a31;
    m[3][2] = a32;
    m[3][3] = a33;
  }

  // constructor from Assimp matrix
  Matrix4f(const aiMatrix4x4 &AssimpMatrix) {
    m[0][0] = AssimpMatrix.a1;
    m[0][1] = AssimpMatrix.a2;
    m[0][2] = AssimpMatrix.a3;
    m[0][3] = AssimpMatrix.a4;
    m[1][0] = AssimpMatrix.b1;
    m[1][1] = AssimpMatrix.b2;
    m[1][2] = AssimpMatrix.b3;
    m[1][3] = AssimpMatrix.b4;
    m[2][0] = AssimpMatrix.c1;
    m[2][1] = AssimpMatrix.c2;
    m[2][2] = AssimpMatrix.c3;
    m[2][3] = AssimpMatrix.c4;
    m[3][0] = AssimpMatrix.d1;
    m[3][1] = AssimpMatrix.d2;
    m[3][2] = AssimpMatrix.d3;
    m[3][3] = AssimpMatrix.d4;
  }

  Matrix4f(const aiMatrix3x3 &AssimpMatrix) {
    m[0][0] = AssimpMatrix.a1;
    m[0][1] = AssimpMatrix.a2;
    m[0][2] = AssimpMatrix.a3;
    m[0][3] = 0.0f;
    m[1][0] = AssimpMatrix.b1;
    m[1][1] = AssimpMatrix.b2;
    m[1][2] = AssimpMatrix.b3;
    m[1][3] = 0.0f;
    m[2][0] = AssimpMatrix.c1;
    m[2][1] = AssimpMatrix.c2;
    m[2][2] = AssimpMatrix.c3;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
  }

  void SetZero() { ZERO_MEM(m); }

  Matrix4f Transpose() const {
    Matrix4f n;

    for (unsigned int i = 0; i < 4; i++) {
      for (unsigned int j = 0; j < 4; j++) {
        n.m[i][j] = m[j][i];
      }
    }

    return n;
  }

  inline void InitIdentity() {
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
  }

  inline Matrix4f operator*(const Matrix4f &Right) const {
    Matrix4f Ret;

    for (unsigned int i = 0; i < 4; i++) {
      for (unsigned int j = 0; j < 4; j++) {
        Ret.m[i][j] = m[i][0] * Right.m[0][j] + m[i][1] * Right.m[1][j] +
                      m[i][2] * Right.m[2][j] + m[i][3] * Right.m[3][j];
      }
    }

    return Ret;
  }

  Vector4f operator*(const Vector4f &v) const {
    Vector4f r;

    r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    r.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

    return r;
  }

  operator const float *() const { return &(m[0][0]); }

  void Print() const {
    for (int i = 0; i < 4; i++) {
      printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
    }
  }

  float Determinant() const;

  Matrix4f Inverse() const;

  void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
  void InitScaleTransform(float Scale);
  void InitScaleTransform(const Vector3f &Scale);

  void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
  void InitRotateTransformZYX(float RotateX, float RotateY, float RotateZ);
  void InitRotateTransform(const Vector3f &Rotate);
  void InitRotateTransform(const Quaternion &quat);

  void InitTranslationTransform(float x, float y, float z);
  void InitTranslationTransform(const Vector3f &Pos);

  void InitCameraTransform(const Vector3f &Target, const Vector3f &Up);

  void InitCameraTransform(const Vector3f &Pos, const Vector3f &Target,
                           const Vector3f &Up);

  void InitPersProjTransform(const PersProjInfo &p);

  void InitOrthoProjTransform(const OrthoProjInfo &p);

  void CalcClipPlanes(Vector4f &l, Vector4f &r, Vector4f &b, Vector4f &t,
                      Vector4f &n, Vector4f &f) const;

private:
  void InitRotationX(float RotateX);
  void InitRotationY(float RotateY);
  void InitRotationZ(float RotateZ);
};

class Matrix3f {
public:
  float m[3][3];

  Matrix3f() {}

  // Initialize the matrix from the top left corner of the 4-by-4 matrix
  Matrix3f(const Matrix4f &a) {
    m[0][0] = a.m[0][0];
    m[0][1] = a.m[0][1];
    m[0][2] = a.m[0][2];
    m[1][0] = a.m[1][0];
    m[1][1] = a.m[1][1];
    m[1][2] = a.m[1][2];
    m[2][0] = a.m[2][0];
    m[2][1] = a.m[2][1];
    m[2][2] = a.m[2][2];
  }

  Vector3f operator*(const Vector3f &v) const {
    Vector3f r;

    r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;

    return r;
  }

  inline Matrix3f operator*(const Matrix3f &Right) const {
    Matrix3f Ret;

    for (unsigned int i = 0; i < 3; i++) {
      for (unsigned int j = 0; j < 3; j++) {
        Ret.m[i][j] = m[i][0] * Right.m[0][j] + m[i][1] * Right.m[1][j] +
                      m[i][2] * Right.m[2][j];
      }
    }

    return Ret;
  }

  Matrix3f Transpose() const {
    Matrix3f n;

    for (unsigned int i = 0; i < 3; i++) {
      for (unsigned int j = 0; j < 3; j++) {
        n.m[i][j] = m[j][i];
      }
    }

    return n;
  }

  void Print() const {
    for (int i = 0; i < 3; i++) {
      printf("%f %f %f\n", m[i][0], m[i][1], m[i][2]);
    }
  }
};

class AABB {
public:
  AABB() {}

  void Add(const Vector3f &v) {
    MinX = min(MinX, v.x);
    MinY = min(MinY, v.y);
    MinZ = min(MinZ, v.z);

    MaxX = max(MaxX, v.x);
    MaxY = max(MaxY, v.y);
    MaxZ = max(MaxZ, v.z);
  }

  float MinX = FLT_MAX;
  float MaxX = FLT_MIN;
  float MinY = FLT_MAX;
  float MaxY = FLT_MIN;
  float MinZ = FLT_MAX;
  float MaxZ = FLT_MIN;

  void Print() {
    printf("X: [%f,%f]\n", MinX, MaxX);
    printf("Y: [%f,%f]\n", MinY, MaxY);
    printf("Z: [%f,%f]\n", MinZ, MaxZ);
  }

  void UpdateOrthoInfo(struct OrthoProjInfo &o) {
    o.r = MaxX;
    o.l = MinX;
    o.b = MinY;
    o.t = MaxY;
    o.n = MinZ;
    o.f = MaxZ;
  }
};

class Frustum {
public:
  Vector4f NearTopLeft;
  Vector4f NearBottomLeft;
  Vector4f NearTopRight;
  Vector4f NearBottomRight;

  Vector4f FarTopLeft;
  Vector4f FarBottomLeft;
  Vector4f FarTopRight;
  Vector4f FarBottomRight;

  Frustum() {}

  void CalcCorners(const PersProjInfo &persProjInfo) {
    float AR = persProjInfo.Height / persProjInfo.Width;

    float tanHalfFOV = tanf(ToRadian(persProjInfo.FOV / 2.0f));

    float NearZ = persProjInfo.zNear;
    float NearX = NearZ * tanHalfFOV;
    float NearY = NearZ * tanHalfFOV * AR;

    NearTopLeft = Vector4f(-NearX, NearY, NearZ, 1.0f);
    NearBottomLeft = Vector4f(-NearX, -NearY, NearZ, 1.0f);
    NearTopRight = Vector4f(NearX, NearY, NearZ, 1.0f);
    NearBottomRight = Vector4f(NearX, -NearY, NearZ, 1.0f);

    float FarZ = persProjInfo.zFar;
    float FarX = FarZ * tanHalfFOV;
    float FarY = FarZ * tanHalfFOV * AR;

    FarTopLeft = Vector4f(-FarX, FarY, FarZ, 1.0f);
    FarBottomLeft = Vector4f(-FarX, -FarY, FarZ, 1.0f);
    FarTopRight = Vector4f(FarX, FarY, FarZ, 1.0f);
    FarBottomRight = Vector4f(FarX, -FarY, FarZ, 1.0f);
  }

  void Transform(const Matrix4f &m) {
    NearTopLeft = m * NearTopLeft;
    NearBottomLeft = m * NearBottomLeft;
    NearTopRight = m * NearTopRight;
    NearBottomRight = m * NearBottomRight;

    FarTopLeft = m * FarTopLeft;
    FarBottomLeft = m * FarBottomLeft;
    FarTopRight = m * FarTopRight;
    FarBottomRight = m * FarBottomRight;
  }

  void CalcAABB(AABB &aabb) {
    aabb.Add(NearTopLeft);
    aabb.Add(NearBottomLeft);
    aabb.Add(NearTopRight);
    aabb.Add(NearBottomRight);

    aabb.Add(FarTopLeft);
    aabb.Add(FarBottomLeft);
    aabb.Add(FarTopRight);
    aabb.Add(FarBottomRight);
  }

  void Print() {
    printf("NearTopLeft ");
    NearTopLeft.Print();
    printf("NearBottomLeft ");
    NearBottomLeft.Print();
    printf("NearTopRight ");
    NearTopRight.Print();
    printf("NearBottomLeft ");
    NearBottomRight.Print();

    printf("FarTopLeft ");
    FarTopLeft.Print();
    printf("FarBottomLeft ");
    FarBottomLeft.Print();
    printf("FarTopRight ");
    FarTopRight.Print();
    printf("FarBottomLeft ");
    FarBottomRight.Print();
  }
};

class FrustumCulling {
public:
  FrustumCulling(const Matrix4f &ViewProj) { Update(ViewProj); }

  void Update(const Matrix4f &ViewProj) {
    ViewProj.CalcClipPlanes(m_leftClipPlane, m_rightClipPlane,
                            m_bottomClipPlane, m_topClipPlane, m_nearClipPlane,
                            m_farClipPlane);
  }

  bool IsPointInsideViewFrustum(const Vector3f &p) const {
    Vector4f p4D(p, 1.0f);

    bool Inside =
        (m_leftClipPlane.Dot(p4D) >= 0) && (m_rightClipPlane.Dot(p4D) <= 0) &&
        //   (m_topClipPlane.Dot(p4D)    <= 0) &&
        //   (m_bottomClipPlane.Dot(p4D) >= 0) &&
        (m_nearClipPlane.Dot(p4D) >= 0) && (m_farClipPlane.Dot(p4D) <= 0);

    return Inside;
  }

private:
  Vector4f m_leftClipPlane;
  Vector4f m_rightClipPlane;
  Vector4f m_bottomClipPlane;
  Vector4f m_topClipPlane;
  Vector4f m_nearClipPlane;
  Vector4f m_farClipPlane;
};

void CalcTightLightProjection(const Matrix4f &CameraView,       // in
                              const Vector3f &LightDir,         // in
                              const PersProjInfo &persProjInfo, // in
                              Vector3f &LightPosWorld,          // out
                              OrthoProjInfo &orthoProjInfo);    // out

int CalcNextPowerOfTwo(int x);

bool IsPointInsideViewFrustum(const Vector3f &p, const Matrix4f &VP);

// IMP
Vector4f &Vector4f::Normalize() {
  float len = Length();

  assert(len != 0);

  x /= len;
  y /= len;
  z /= len;
  w /= len;

  return *this;
}

Vector3f Vector3f::Cross(const Vector3f &v) const {
  const float _x = y * v.z - z * v.y;
  const float _y = z * v.x - x * v.z;
  const float _z = x * v.y - y * v.x;

  return Vector3f(_x, _y, _z);
}

Vector3f &Vector3f::Normalize() {
  float len = Length();

  assert(len != 0);

  x /= len;
  y /= len;
  z /= len;

  return *this;
}

void Vector3f::Rotate(float Angle, const Vector3f &V) {
  Quaternion RotationQ(Angle, V);

  Quaternion ConjugateQ = RotationQ.Conjugate();

  Quaternion W = RotationQ * (*this) * ConjugateQ;

  x = W.x;
  y = W.y;
  z = W.z;
}

Vector3f Vector3f::Negate() const {
  Vector3f ret(-x, -y, -z);
  return ret;
}

void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
  m[0][0] = ScaleX;
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0f;
  m[1][0] = 0.0f;
  m[1][1] = ScaleY;
  m[1][2] = 0.0f;
  m[1][3] = 0.0f;
  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = ScaleZ;
  m[2][3] = 0.0f;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

void Matrix4f::InitScaleTransform(float Scale) {
  InitScaleTransform(Scale, Scale, Scale);
}

void Matrix4f::InitScaleTransform(const Vector3f &Scale) {
  InitScaleTransform(Scale.x, Scale.y, Scale.z);
}

void Matrix4f::InitRotateTransform(float RotateX, float RotateY,
                                   float RotateZ) {
  Matrix4f rx, ry, rz;

  float x = ToRadian(RotateX);
  float y = ToRadian(RotateY);
  float z = ToRadian(RotateZ);

  rx.InitRotationX(x);
  ry.InitRotationY(y);
  rz.InitRotationZ(z);

  *this = rz * ry * rx;
}

void Matrix4f::InitRotateTransformZYX(float RotateX, float RotateY,
                                      float RotateZ) {
  Matrix4f rx, ry, rz;

  float x = ToRadian(RotateX);
  float y = ToRadian(RotateY);
  float z = ToRadian(RotateZ);

  rx.InitRotationX(x);
  ry.InitRotationY(y);
  rz.InitRotationZ(z);

  *this = rx * ry * rz;
}

void Matrix4f::InitRotateTransform(const Vector3f &Rotate) {
  InitRotateTransform(Rotate.x, Rotate.y, Rotate.z);
}

//
// The following rotation matrices are for a left handed coordinate system.
// https://butterflyofdream.wordpress.com/2016/07/05/converting-rotation-matrices-of-left-handed-coordinate-system/
//

void Matrix4f::InitRotationX(float x) {
  m[0][0] = 1.0f;
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0f;
  m[1][0] = 0.0f;
  m[1][1] = cosf(x);
  m[1][2] = sinf(x);
  m[1][3] = 0.0f;
  m[2][0] = 0.0f;
  m[2][1] = -sinf(x);
  m[2][2] = cosf(x);
  m[2][3] = 0.0f;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

void Matrix4f::InitRotationY(float y) {
  m[0][0] = cosf(y);
  m[0][1] = 0.0f;
  m[0][2] = -sinf(y);
  m[0][3] = 0.0f;
  m[1][0] = 0.0f;
  m[1][1] = 1.0f;
  m[1][2] = 0.0f;
  m[1][3] = 0.0f;
  m[2][0] = sinf(y);
  m[2][1] = 0.0f;
  m[2][2] = cosf(y);
  m[2][3] = 0.0f;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

void Matrix4f::InitRotationZ(float z) {
  m[0][0] = cosf(z);
  m[0][1] = sinf(z);
  m[0][2] = 0.0f;
  m[0][3] = 0.0f;
  m[1][0] = -sinf(z);
  m[1][1] = cosf(z);
  m[1][2] = 0.0f;
  m[1][3] = 0.0f;
  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = 1.0f;
  m[2][3] = 0.0f;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(const Quaternion &quat) {
  float yy2 = 2.0f * quat.y * quat.y;
  float xy2 = 2.0f * quat.x * quat.y;
  float xz2 = 2.0f * quat.x * quat.z;
  float yz2 = 2.0f * quat.y * quat.z;
  float zz2 = 2.0f * quat.z * quat.z;
  float wz2 = 2.0f * quat.w * quat.z;
  float wy2 = 2.0f * quat.w * quat.y;
  float wx2 = 2.0f * quat.w * quat.x;
  float xx2 = 2.0f * quat.x * quat.x;
  m[0][0] = -yy2 - zz2 + 1.0f;
  m[0][1] = xy2 + wz2;
  m[0][2] = xz2 - wy2;
  m[0][3] = 0;
  m[1][0] = xy2 - wz2;
  m[1][1] = -xx2 - zz2 + 1.0f;
  m[1][2] = yz2 + wx2;
  m[1][3] = 0;
  m[2][0] = xz2 + wy2;
  m[2][1] = yz2 - wx2;
  m[2][2] = -xx2 - yy2 + 1.0f;
  m[2][3] = 0.0f;
  m[3][0] = m[3][1] = m[3][2] = 0;
  m[3][3] = 1.0f;
}

void Matrix4f::InitTranslationTransform(float x, float y, float z) {
  m[0][0] = 1.0f;
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = x;
  m[1][0] = 0.0f;
  m[1][1] = 1.0f;
  m[1][2] = 0.0f;
  m[1][3] = y;
  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = 1.0f;
  m[2][3] = z;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

void Matrix4f::InitTranslationTransform(const Vector3f &Pos) {
  InitTranslationTransform(Pos.x, Pos.y, Pos.z);
}

void Matrix4f::InitCameraTransform(const Vector3f &Target, const Vector3f &Up) {
  Vector3f N = Target;
  N.Normalize();

  Vector3f UpNorm = Up;
  UpNorm.Normalize();

  Vector3f U;
  U = UpNorm.Cross(N);
  U.Normalize();

  Vector3f V = N.Cross(U);

  m[0][0] = U.x;
  m[0][1] = U.y;
  m[0][2] = U.z;
  m[0][3] = 0.0f;
  m[1][0] = V.x;
  m[1][1] = V.y;
  m[1][2] = V.z;
  m[1][3] = 0.0f;
  m[2][0] = N.x;
  m[2][1] = N.y;
  m[2][2] = N.z;
  m[2][3] = 0.0f;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

void Matrix4f::InitCameraTransform(const Vector3f &Pos, const Vector3f &Target,
                                   const Vector3f &Up) {
  Matrix4f CameraTranslation;
  CameraTranslation.InitTranslationTransform(-Pos.x, -Pos.y, -Pos.z);

  Matrix4f CameraRotateTrans;
  CameraRotateTrans.InitCameraTransform(Target, Up);

  *this = CameraRotateTrans * CameraTranslation;
}

void Matrix4f::InitPersProjTransform(const PersProjInfo &p) {
  float ar = p.Height / p.Width;
  float zRange = p.zNear - p.zFar;
  float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));

  m[0][0] = 1 / tanHalfFOV;
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0;
  m[1][0] = 0.0f;
  m[1][1] = 1.0f / (tanHalfFOV * ar);
  m[1][2] = 0.0f;
  m[1][3] = 0.0;
  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = (-p.zNear - p.zFar) / zRange;
  m[2][3] = 2.0f * p.zFar * p.zNear / zRange;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 1.0f;
  m[3][3] = 0.0;

#ifdef USE_GLM
  glm::mat4 Projection = glm::perspectiveFovLH(glm::radians(p.FOV), p.Width,
                                               p.Height, p.zNear, p.zFar);

  m[0][0] = Projection[0][0];
  m[0][1] = Projection[1][0];
  m[0][2] = Projection[2][0];
  m[0][3] = Projection[3][0];
  m[1][0] = Projection[0][1];
  m[1][1] = Projection[1][1];
  m[1][2] = Projection[2][1];
  m[1][3] = Projection[3][1];
  m[2][0] = Projection[0][2];
  m[2][1] = Projection[1][2];
  m[2][2] = Projection[2][2];
  m[2][3] = Projection[3][2];
  m[3][0] = Projection[0][3];
  m[3][1] = Projection[1][3];
  m[3][2] = Projection[2][3];
  m[3][3] = Projection[3][3];
#endif

  //    std::cout << glm::to_string(Projection) << std::endl;
}

void Matrix4f::InitOrthoProjTransform(const OrthoProjInfo &p) {
  float l = p.l;
  float r = p.r;
  float b = p.b;
  float t = p.t;
  float n = p.n;
  float f = p.f;

  m[0][0] = 2.0f / (r - l);
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = -(r + l) / (r - l);
  m[1][0] = 0.0f;
  m[1][1] = 2.0f / (t - b);
  m[1][2] = 0.0f;
  m[1][3] = -(t + b) / (t - b);
  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = 2.0f / (f - n);
  m[2][3] = -(f + n) / (f - n);
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0;
}

float Matrix4f::Determinant() const {
  return m[0][0] * m[1][1] * m[2][2] * m[3][3] -
         m[0][0] * m[1][1] * m[2][3] * m[3][2] +
         m[0][0] * m[1][2] * m[2][3] * m[3][1] -
         m[0][0] * m[1][2] * m[2][1] * m[3][3] +
         m[0][0] * m[1][3] * m[2][1] * m[3][2] -
         m[0][0] * m[1][3] * m[2][2] * m[3][1] -
         m[0][1] * m[1][2] * m[2][3] * m[3][0] +
         m[0][1] * m[1][2] * m[2][0] * m[3][3] -
         m[0][1] * m[1][3] * m[2][0] * m[3][2] +
         m[0][1] * m[1][3] * m[2][2] * m[3][0] -
         m[0][1] * m[1][0] * m[2][2] * m[3][3] +
         m[0][1] * m[1][0] * m[2][3] * m[3][2] +
         m[0][2] * m[1][3] * m[2][0] * m[3][1] -
         m[0][2] * m[1][3] * m[2][1] * m[3][0] +
         m[0][2] * m[1][0] * m[2][1] * m[3][3] -
         m[0][2] * m[1][0] * m[2][3] * m[3][1] +
         m[0][2] * m[1][1] * m[2][3] * m[3][0] -
         m[0][2] * m[1][1] * m[2][0] * m[3][3] -
         m[0][3] * m[1][0] * m[2][1] * m[3][2] +
         m[0][3] * m[1][0] * m[2][2] * m[3][1] -
         m[0][3] * m[1][1] * m[2][2] * m[3][0] +
         m[0][3] * m[1][1] * m[2][0] * m[3][2] -
         m[0][3] * m[1][2] * m[2][0] * m[3][1] +
         m[0][3] * m[1][2] * m[2][1] * m[3][0];
}

Matrix4f Matrix4f::Inverse() const {
  // Compute the reciprocal determinant
  float det = Determinant();

  if (det == 0.0f) {
    assert(0);
    return *this;
  }

  float invdet = 1.0f / det;

  Matrix4f res;
  res.m[0][0] = invdet * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) +
                          m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) +
                          m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
  res.m[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) +
                           m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) +
                           m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
  res.m[0][2] = invdet * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
                          m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) +
                          m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
  res.m[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) +
                           m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) +
                           m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
  res.m[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) +
                           m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) +
                           m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
  res.m[1][1] = invdet * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) +
                          m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) +
                          m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
  res.m[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
                           m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) +
                           m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
  res.m[1][3] = invdet * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) +
                          m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) +
                          m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
  res.m[2][0] = invdet * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) +
                          m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) +
                          m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) +
                           m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) +
                           m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[2][2] = invdet * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
                          m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) +
                          m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
  res.m[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) +
                           m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) +
                           m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
  res.m[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) +
                           m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) +
                           m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[3][1] = invdet * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) +
                          m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) +
                          m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
                           m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) +
                           m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
  res.m[3][3] = invdet * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) +
                          m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) +
                          m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
  return res;
}

void Matrix4f::CalcClipPlanes(Vector4f &l, Vector4f &r, Vector4f &b,
                              Vector4f &t, Vector4f &n, Vector4f &f) const {
  Vector4f Row1(m[0][0], m[0][1], m[0][2], m[0][3]);
  Vector4f Row2(m[1][0], m[1][1], m[1][2], m[1][3]);
  Vector4f Row3(m[2][0], m[2][1], m[2][2], m[2][3]);
  Vector4f Row4(m[3][0], m[3][1], m[3][2], m[3][3]);

  l = Row1 + Row4;
  r = Row1 - Row4;
  b = Row2 + Row4;
  t = Row2 - Row4;
  n = Row3 + Row4;
  f = Row3 - Row4;
}

Quaternion::Quaternion(float Angle, const Vector3f &V) {
  float HalfAngleInRadians = ToRadian(Angle / 2);

  float SineHalfAngle = sinf(HalfAngleInRadians);
  float CosHalfAngle = cosf(HalfAngleInRadians);

  x = V.x * SineHalfAngle;
  y = V.y * SineHalfAngle;
  z = V.z * SineHalfAngle;
  w = CosHalfAngle;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) {
  x = _x;
  y = _y;
  z = _z;
  w = _w;
}

void Quaternion::Normalize() {
  float Length = sqrtf(x * x + y * y + z * z + w * w);

  x /= Length;
  y /= Length;
  z /= Length;
  w /= Length;
}

Quaternion Quaternion::Conjugate() const {
  Quaternion ret(-x, -y, -z, w);
  return ret;
}

Quaternion operator*(const Quaternion &q, const Vector3f &v) {
  float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
  float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
  float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
  float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

  Quaternion ret(x, y, z, w);

  return ret;
}

Quaternion operator*(const Quaternion &l, const Quaternion &r) {
  float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
  float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
  float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
  float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

  Quaternion ret(x, y, z, w);

  return ret;
}

Vector3f Quaternion::ToDegrees() {
  float f[3];

  f[0] = atan2(x * z + y * w, x * w - y * z);
  f[1] = acos(-x * x - y * y - z * z - w * w);
  f[2] = atan2(x * z - y * w, x * w + y * z);

  f[0] = ToDegree(f[0]);
  f[1] = ToDegree(f[1]);
  f[2] = ToDegree(f[2]);

  return Vector3f(f);
}

float RandomFloat() {
  float Max = RAND_MAX;
  return ((float)RANDOM() / Max);
}

float RandomFloatRange(float Start, float End) {
  if (End == Start) {
    printf("Invalid random range: (%f, %f)\n", Start, End);
    exit(0);
  }

  float Delta = End - Start;

  float RandomValue = RandomFloat() * Delta + Start;

  return RandomValue;
}

void CalcTightLightProjection(const Matrix4f &CameraView,       // in
                              const Vector3f &LightDir,         // in
                              const PersProjInfo &persProjInfo, // in
                              Vector3f &LightPosWorld,          // out
                              OrthoProjInfo &orthoProjInfo)     // out
{
  //
  // Step #1: Calculate the view frustum corners in view space
  //
  Frustum frustum;
  frustum.CalcCorners(persProjInfo);

  //
  // Step #2: transform the view frustum to world space
  //
  Matrix4f InverseCameraView = CameraView.Inverse();
  frustum.Transform(InverseCameraView);

  Frustum view_frustum_in_world_space = frustum; // backup for later

  //
  // Step #3: Transform the view frustum to light space (1st time)
  //
  Matrix4f LightView;
  Vector3f Origin(0.0f, 0.0f, 0.0f);
  Vector3f Up(0.0f, 1.0f, 0.0f);
  LightView.InitCameraTransform(Origin, LightDir, Up);
  frustum.Transform(LightView);

  //
  // Step #4: Calculate an AABB for the view frustum in light space
  //
  AABB aabb;
  frustum.CalcAABB(aabb);

  //
  // Step #5: Calculate the position of the light
  //
  Vector3f BottomLeft(aabb.MinX, aabb.MinY, aabb.MinZ);
  Vector3f TopRight(aabb.MaxX, aabb.MaxY, aabb.MinZ);
  Vector4f LightPosWorld4d = Vector4f((BottomLeft + TopRight) / 2.0f, 1.0f);

  //
  // Step #6: transform the position of the light back to world space
  //
  Matrix4f LightViewInv = LightView.Inverse();
  LightPosWorld4d = LightViewInv * LightPosWorld4d;
  LightPosWorld =
      Vector3f(LightPosWorld4d.x, LightPosWorld4d.y, LightPosWorld4d.z);

  //
  // Step #7: transform the view frustum to light space (2nd time)
  //
  LightView.InitCameraTransform(LightPosWorld, LightDir, Up);
  view_frustum_in_world_space.Transform(LightView);

  //
  // Step #8: with the light in its final position recalculate the aabb
  //
  AABB final_aabb;
  view_frustum_in_world_space.CalcAABB(final_aabb);
  final_aabb.UpdateOrthoInfo(orthoProjInfo);
}

int CalcNextPowerOfTwo(int x) {
  int ret = 1;

  if (x == 1) {
    return 2;
  }

  while (ret < x) {
    ret = ret * 2;
  }

  return ret;
}

bool IsPointInsideViewFrustum(const Vector3f &p, const Matrix4f &VP) {
  Vector4f p4D(p, 1.0f);

  Vector4f ClipSpaceP = VP * p4D;

  bool InsideViewFrustum =
      ((ClipSpaceP.x <= ClipSpaceP.w) && (ClipSpaceP.x >= -ClipSpaceP.w) &&
       (ClipSpaceP.y <= ClipSpaceP.w) && (ClipSpaceP.y >= -ClipSpaceP.w) &&
       (ClipSpaceP.z <= ClipSpaceP.w) && (ClipSpaceP.z >= -ClipSpaceP.w));

  return InsideViewFrustum;
}