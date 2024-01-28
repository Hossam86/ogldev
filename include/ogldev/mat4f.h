#pragma once

#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>

#include <ogldev/quat.h>
#include <ogldev/utility.h>
#include <ogldev/vec4f.h>

class Matrix4f
{
public:
	float m[4][4];

	Matrix4f() {}

	Matrix4f(
		float a00,
		float a01,
		float a02,
		float a03,
		float a10,
		float a11,
		float a12,
		float a13,
		float a20,
		float a21,
		float a22,
		float a23,
		float a30,
		float a31,
		float a32,
		float a33)
	{
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
	Matrix4f(const aiMatrix4x4& AssimpMatrix)
	{
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

	Matrix4f(const aiMatrix3x3& AssimpMatrix)
	{
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

	void
	SetZero()
	{
		ZERO_MEM(m);
	}

	Matrix4f
	Transpose() const
	{
		Matrix4f n;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				n.m[i][j] = m[j][i];
			}
		}

		return n;
	}

	inline void
	InitIdentity()
	{
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

	inline Matrix4f
	operator*(const Matrix4f& Right) const
	{
		Matrix4f Ret;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				Ret.m[i][j] = m[i][0] * Right.m[0][j] + m[i][1] * Right.m[1][j] + m[i][2] * Right.m[2][j] +
							  m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}

	Vector4f
	operator*(const Vector4f& v) const
	{
		Vector4f r;

		r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
		r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
		r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
		r.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

		return r;
	}

	operator const float*() const { return &(m[0][0]); }

	void
	Print() const
	{
		for (int i = 0; i < 4; i++)
		{
			printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
		}
	}

	float
	Determinant() const
	{
		return m[0][0] * m[1][1] * m[2][2] * m[3][3] - m[0][0] * m[1][1] * m[2][3] * m[3][2] +
			   m[0][0] * m[1][2] * m[2][3] * m[3][1] - m[0][0] * m[1][2] * m[2][1] * m[3][3] +
			   m[0][0] * m[1][3] * m[2][1] * m[3][2] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
			   m[0][1] * m[1][2] * m[2][3] * m[3][0] + m[0][1] * m[1][2] * m[2][0] * m[3][3] -
			   m[0][1] * m[1][3] * m[2][0] * m[3][2] + m[0][1] * m[1][3] * m[2][2] * m[3][0] -
			   m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][1] * m[1][0] * m[2][3] * m[3][2] +
			   m[0][2] * m[1][3] * m[2][0] * m[3][1] - m[0][2] * m[1][3] * m[2][1] * m[3][0] +
			   m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][2] * m[1][0] * m[2][3] * m[3][1] +
			   m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][2] * m[1][1] * m[2][0] * m[3][3] -
			   m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][3] * m[1][0] * m[2][2] * m[3][1] -
			   m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][3] * m[1][1] * m[2][0] * m[3][2] -
			   m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][3] * m[1][2] * m[2][1] * m[3][0];
	}

	Matrix4f
	Inverse() const
	{
		// Compute the reciprocal determinant
		float det = Determinant();

		if (det == 0.0f)
		{
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

	void
	InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
	{
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

	void
	InitScaleTransform(float Scale)
	{
		InitScaleTransform(Scale, Scale, Scale);
	}

	void
	InitScaleTransform(const Vector3f& Scale)
	{
		InitScaleTransform(Scale.x, Scale.y, Scale.z);
	}

	void
	InitRotateTransform(float RotateX, float RotateY, float RotateZ)
	{
		Matrix4f rx, ry, rz;

		float x = ToRadian(RotateX);
		float y = ToRadian(RotateY);
		float z = ToRadian(RotateZ);

		rx.InitRotationX(x);
		ry.InitRotationY(y);
		rz.InitRotationZ(z);

		*this = rz * ry * rx;
	}

	void
	InitRotateTransformZYX(float RotateX, float RotateY, float RotateZ)
	{
		Matrix4f rx, ry, rz;

		float x = ToRadian(RotateX);
		float y = ToRadian(RotateY);
		float z = ToRadian(RotateZ);

		rx.InitRotationX(x);
		ry.InitRotationY(y);
		rz.InitRotationZ(z);

		*this = rx * ry * rz;
	}

	void
	InitRotateTransform(const Vector3f& Rotate)
	{
		InitRotateTransform(Rotate.x, Rotate.y, Rotate.z);
	}
	void
	InitRotateTransform(const Quaternion& quat)
	{
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

	void
	InitTranslationTransform(float x, float y, float z)
	{
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

	void
	InitTranslationTransform(const Vector3f& Pos)
	{
		InitTranslationTransform(Pos.x, Pos.y, Pos.z);
	}

	void
	InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
	{
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

	void
	InitCameraTransform(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
	{
		Matrix4f CameraTranslation;
		CameraTranslation.InitTranslationTransform(-Pos.x, -Pos.y, -Pos.z);

		Matrix4f CameraRotateTrans;
		CameraRotateTrans.InitCameraTransform(Target, Up);

		*this = CameraRotateTrans * CameraTranslation;
	}

	void
	InitPersProjTransform(const PersProjInfo& p)
	{
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
		glm::mat4 Projection = glm::perspectiveFovLH(glm::radians(p.FOV), p.Width, p.Height, p.zNear, p.zFar);

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

	void
	InitOrthoProjTransform(const OrthoProjInfo& p)
	{
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

	void
	CalcClipPlanes(Vector4f& l, Vector4f& r, Vector4f& b, Vector4f& t, Vector4f& n, Vector4f& f) const
	{
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

private:
	//
	// The following rotation matrices are for a left handed coordinate system.
	// https://butterflyofdream.wordpress.com/2016/07/05/converting-rotation-matrices-of-left-handed-coordinate-system/
	//
	void
	InitRotationX(float RotateX)
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;
		m[1][0] = 0.0f;
		m[1][1] = cosf(RotateX);
		m[1][2] = sinf(RotateX);
		m[1][3] = 0.0f;
		m[2][0] = 0.0f;
		m[2][1] = -sinf(RotateX);
		m[2][2] = cosf(RotateX);
		m[2][3] = 0.0f;
		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	void
	InitRotationY(float RotateY)
	{
		m[0][0] = cosf(RotateY);
		m[0][1] = 0.0f;
		m[0][2] = -sinf(RotateY);
		m[0][3] = 0.0f;
		m[1][0] = 0.0f;
		m[1][1] = 1.0f;
		m[1][2] = 0.0f;
		m[1][3] = 0.0f;
		m[2][0] = sinf(RotateY);
		m[2][1] = 0.0f;
		m[2][2] = cosf(RotateY);
		m[2][3] = 0.0f;
		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	void
	InitRotationZ(float RotateZ)
	{
		m[0][0] = cosf(RotateZ);
		m[0][1] = sinf(RotateZ);
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;
		m[1][0] = -sinf(RotateZ);
		m[1][1] = cosf(RotateZ);
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
	};
};
