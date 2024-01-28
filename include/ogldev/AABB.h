#pragma once

#include <cstdio>
#include <float.h>
#include <limits.h>
#include <math.h>

#include <ogldev/Vector3f.h>

// struct PersProjInfo
// {
// 	float FOV;
// 	float Width;
// 	float Height;
// 	float zNear;
// 	float zFar;
// };

class AABB
{
public:
	AABB() {}

	void
	Add(const Vector3f& v)
	{
		MinX = fmin(MinX, v.x);
		MinY = fmin(MinY, v.y);
		MinZ = fmin(MinZ, v.z);

		MaxX = fmax(MaxX, v.x);
		MaxY = fmax(MaxY, v.y);
		MaxZ = fmax(MaxZ, v.z);
	}

	float MinX = FLT_MAX;
	float MaxX = FLT_MIN;
	float MinY = FLT_MAX;
	float MaxY = FLT_MIN;
	float MinZ = FLT_MAX;
	float MaxZ = FLT_MIN;

	void
	Print()
	{
		printf("X: [%f,%f]\n", MinX, MaxX);
		printf("Y: [%f,%f]\n", MinY, MaxY);
		printf("Z: [%f,%f]\n", MinZ, MaxZ);
	}

	void
	UpdateOrthoInfo(struct OrthoProjInfo& o)
	{
		o.r = MaxX;
		o.l = MinX;
		o.b = MinY;
		o.t = MaxY;
		o.n = MinZ;
		o.f = MaxZ;
	}
};