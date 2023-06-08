#pragma once

#include "utils.h"

#include <cstdint>

struct tuple
{
	tuple()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f; 
		w = 0.0f;
	}

	tuple(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	float operator[](int32_t index) const
	{
		return data[index];
	}

	float& operator[](int32_t index)
	{
		return data[index];
	}

	bool isPoint() { return equal(w, 1.0f); }
	bool isVector() { return equal(w, 0.0f); }

	union
	{
		struct
		{
			float data[4];
		};

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float red;
			float green;
			float blue;
			float alpha;
		};
	};
};

inline tuple point(float x, float y, float z)
{
	return tuple(x, y, z, 1.0f);
}

inline tuple vector(float x, float y, float z)
{
	return tuple(x, y, z, 0.0f);
}

inline tuple operator+(const tuple& a, const tuple& b)
{
	return tuple(a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w);
}

inline tuple operator+=(tuple& a, const tuple& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;

	return a;
}

inline tuple operator-(const tuple& a, const tuple& b)
{
	return tuple(a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w);
}

inline tuple operator-(const tuple& a)
{
	return tuple(-a.x, -a.y, -a.z, -a.w);
}

inline tuple operator*(const tuple& a, float b)
{
	return tuple(a.x * b, a.y * b, a.z * b, a.w * b);
}

inline tuple operator*(float a, const tuple& b)
{
	return b * a;
}

inline tuple operator*(const tuple a, const tuple& b)
{
	return tuple(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline tuple operator*=(tuple& a, float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;

	return a;
}

inline tuple operator/(const tuple& a, float b)
{
	return tuple(a.x / b, a.y / b, a.z / b, a.w / b);
}

inline bool equal(const tuple& a, const tuple& b)
{
	return equal(a.x, b.x) &&
		equal(a.y, b.y) &&
		equal(a.z, b.z) &&
		equal(a.w, b.w);
}

inline bool operator==(const tuple& a, const tuple& b)
{
	return equal(a, b);
}

inline float dot(const tuple& a, const tuple& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline tuple cross(const tuple& a, const tuple& b)
{
	return vector(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

inline float length(const tuple& v)
{
	return std::sqrtf(dot(v, v));
}

inline tuple normalize(const tuple& v)
{
	return v / length(v);
}

inline tuple reflect(const tuple& in, const tuple& normal)
{
	return in - normal * 2.0f * dot(in, normal);
}

inline tuple color(float red, float green, float blue)
{
	return tuple(red, green, blue, 0.0f);
}