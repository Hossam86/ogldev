#pragma once

struct Vector3i
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
};