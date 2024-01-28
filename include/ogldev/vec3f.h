#pragma once
#include <math.h>
#include <ogldev/utility.h>

struct Vector3f
{
	union
	{
		float x;
		float r;
	};

	union
	{
		float y;
		float g;
	};

	union
	{
		float z;
		float b;
	};

	Vector3f() {}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f(const float* pFloat)
	{
		x = pFloat[0];
		y = pFloat[1];
		z = pFloat[2];
	}

	void
	InitBySphericalCoords(float Radius, float Pitch, float Heading)
	{
		x = Radius * cosf(ToRadian(Pitch)) * sinf(ToRadian(Heading));
		y = -Radius * sinf(ToRadian(Pitch));
		z = Radius * cosf(ToRadian(Pitch)) * cosf(ToRadian(Heading));
	}

	Vector3f(float f) { x = y = z = f; }

	// Vector3f(const Vector4f& v)
	// {
	// 	x = v.x;
	// 	y = v.y;
	// 	z = v.z;
	// }

	Vector3f&
	operator+=(const Vector3f& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}

	Vector3f&
	operator-=(const Vector3f& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;

		return *this;
	}

	Vector3f&
	operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	bool
	operator==(const Vector3f& r)
	{
		return ((x == r.x) && (y == r.y) && (z == r.z));
	}

	bool
	operator!=(const Vector3f& r)
	{
		return !(*this == r);
	}

	operator const float*() const { return &(x); }

	Vector3f
	Cross(const Vector3f& v) const
	{
		const float _x = y * v.z - z * v.y;
		const float _y = z * v.x - x * v.z;
		const float _z = x * v.y - y * v.x;

		return Vector3f(_x, _y, _z);
	}

	float
	Dot(const Vector3f& v) const
	{
		float ret = x * v.x + y * v.y + z * v.z;
		return ret;
	}

	float
	Distance(const Vector3f& v) const
	{
		float delta_x = x - v.x;
		float delta_y = y - v.y;
		float delta_z = z - v.z;
		float distance = sqrtf(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
		return distance;
	}

	float
	Length() const
	{
		float len = sqrtf(x * x + y * y + z * z);
		return len;
	}

	bool
	IsZero() const
	{
		return ((x + y + z) == 0.0f);
	}

	Vector3f&
	Normalize();

	Vector3f
	Negate() const
	{
		Vector3f ret(-x, -y, -z);
		return ret;
	}

	void
	Print(bool endl = true) const
	{
		printf("(%f, %f, %f)", x, y, z);

		if (endl)
		{
			printf("\n");
		}
	}
};

inline Vector3f
operator+(const Vector3f& l, const Vector3f& r)
{
	Vector3f Ret(l.x + r.x, l.y + r.y, l.z + r.z);

	return Ret;
}

inline Vector3f
operator-(const Vector3f& l, const Vector3f& r)
{
	Vector3f Ret(l.x - r.x, l.y - r.y, l.z - r.z);

	return Ret;
}

inline Vector3f
operator*(const Vector3f& l, float f)
{
	Vector3f Ret(l.x * f, l.y * f, l.z * f);

	return Ret;
}

inline Vector3f
operator/(const Vector3f& l, float f)
{
	Vector3f Ret(l.x / f, l.y / f, l.z / f);

	return Ret;
}