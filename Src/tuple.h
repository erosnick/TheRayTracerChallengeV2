#pragma once

#include "utils.h"
#include "maths.h"

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

	bool isPoint() { return Math::equal(w, 1.0f); }
	bool isVector() { return Math::equal(w, 0.0f); }

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

inline static tuple point(float x, float y, float z)
{
	return tuple(x, y, z, 1.0f);
}

inline static tuple point(float value)
{
	return tuple(value, value, value, 1.0f);
}

inline static tuple vector(float x, float y, float z)
{
	return tuple(x, y, z, 0.0f);
}

inline static tuple vector(float value)
{
	return tuple(value, value, value, 0.0f);
}

inline static tuple operator+(const tuple& a, const tuple& b)
{
	return tuple(a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w);
}

inline static tuple operator+=(tuple& a, const tuple& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;

	return a;
}

inline static tuple operator-(const tuple& a, const tuple& b)
{
	return tuple(a.x - b.x,
				 a.y - b.y,
				 a.z - b.z,
				 a.w - b.w);
}

inline static tuple operator-(float a, const tuple& b)
{
	return point(a) - b;
}

inline static tuple operator-(const tuple& a)
{
	return tuple(-a.x, -a.y, -a.z, -a.w);
}

inline static tuple operator*(const tuple& a, float b)
{
	return tuple(a.x * b, a.y * b, a.z * b, a.w * b);
}

inline static tuple operator*(float a, const tuple& b)
{
	return b * a;
}

inline static tuple operator*(const tuple a, const tuple& b)
{
	return tuple(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline static tuple operator*=(tuple& a, float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;

	return a;
}

inline static tuple operator/(const tuple& a, float b)
{
	return tuple(a.x / b, a.y / b, a.z / b, a.w / b);
}

inline static tuple operator/(const tuple& a, const tuple& b)
{
	return tuple(a.x / b.x, a.y / b.y, a.z / b.y, a.w / b.w);
}

inline static bool equal(const tuple& a, const tuple& b)
{
	return Math::equal(a.x, b.x) &&
		   Math::equal(a.y, b.y) &&
		   Math::equal(a.z, b.z) &&
		   Math::equal(a.w, b.w);
}

inline static bool operator==(const tuple& a, const tuple& b)
{
	return equal(a, b);
}

inline static float dot(const tuple& a, const tuple& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline static tuple cross(const tuple& a, const tuple& b)
{
	return vector(a.y * b.z - a.z * b.y,
				  a.z * b.x - a.x * b.z,
				  a.x * b.y - a.y * b.x);
}

inline static float length(const tuple& v)
{
	return std::sqrtf(dot(v, v));
}

inline static float lengthSquared(const tuple& v)
{
	return dot(v, v);
}

inline static tuple normalize(const tuple& v)
{
	return v / length(v);
}

inline static tuple reflect(const tuple& in, const tuple& normal)
{
	return in - normal * 2.0f * dot(in, normal);
}

inline static tuple color(float red, float green, float blue)
{
	return tuple(red, green, blue, 0.0f);
}

inline static tuple color(float value)
{
	return tuple(value, value, value, 0.0f);
}

inline static tuple lerp(const tuple& a, const tuple& b, float alpha)
{
	return (1.0f - alpha) * a + alpha * b;
}

inline static tuple pow(const tuple& a, const tuple& b)
{
	return tuple(std::pow(a.x, b.x), 
				 std::pow(a.y, b.y), 
				 std::pow(a.z, b.z), 1.0f);
}

inline static tuple randomVector(float min, float max)
{
	return vector(Math::randomFloat(min, max), Math::randomFloat(min, max), Math::randomFloat(min, max));
}

inline tuple randomInUnitSphere()
{
	while (true)
	{
		auto p = randomVector(-1.0f, 1.0f);
		if (length(p) * length(p) >= 1.0f)
		{
			continue;
		}
		return p;
	}
}

inline tuple randomUnitVector()
{
	return normalize(randomInUnitSphere());
}

inline tuple randomInHemisphere(const tuple& normal)
{
	tuple inUnitSphere = randomInUnitSphere();
	if (dot(inUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
		return inUnitSphere;
	else
		return -inUnitSphere;
}

inline tuple randomInUnitDisk()
{
	while (true)
	{
		auto p = vector(Math::randomFloat(-1.0f, 1.0f), Math::randomFloat(-1.0f, 1.0f), 0.0f);
		if (lengthSquared(p) >= 1.0f) continue;
		return p;
	}
}