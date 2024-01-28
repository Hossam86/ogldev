#pragma once

#include <cassert>
#include <cstdio>
#include <math.h>

struct Vector2f
{
	union
	{
		float x;
		float u;
	};

	union
	{
		float y;
		float v;
	};

	Vector2f() {}

	Vector2f(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	void
	Print(bool endl = true) const
	{
		printf("(%f, %f)", x, y);

		if (endl)
		{
			printf("\n");
		}
	}

	float
	Length() const
	{
		float len = sqrtf(x * x + y * y);
		return len;
	}

	void
	Normalize()
	{
		float len = Length();

		assert(len != 0);

		x /= len;
		y /= len;
	}
};

inline Vector2f
operator*(const Vector2f& l, float f)
{
	Vector2f Ret(l.x * f, l.y * f);

	return Ret;
}