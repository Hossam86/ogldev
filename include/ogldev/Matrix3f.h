#include <cstdio>
#include <ogldev/Matrix4f.h>
#include <ogldev/Vector3f.h>

class Matrix3f
{
public:
	float m[3][3];

	Matrix3f() {}

	// Initialize the matrix from the top left corner of the 4-by-4 matrix
	Matrix3f(const Matrix4f& a)
	{
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

	Vector3f
	operator*(const Vector3f& v) const
	{
		Vector3f r;

		r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
		r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
		r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;

		return r;
	}

	inline Matrix3f
	operator*(const Matrix3f& Right) const
	{
		Matrix3f Ret;

		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				Ret.m[i][j] = m[i][0] * Right.m[0][j] + m[i][1] * Right.m[1][j] + m[i][2] * Right.m[2][j];
			}
		}

		return Ret;
	}

	Matrix3f
	Transpose() const
	{
		Matrix3f n;

		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				n.m[i][j] = m[j][i];
			}
		}

		return n;
	}

	void
	Print() const
	{
		for (int i = 0; i < 3; i++)
		{
			printf("%f %f %f\n", m[i][0], m[i][1], m[i][2]);
		}
	}
};
