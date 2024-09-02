#include "mgdl/wii/mgdl-wii-vector-operations.h"

guVector operator+(const guVector a, const guVector b)
{
	guVector ab;
	guVecAdd(&a, &b, &ab);
	return ab;
}

guVector operator-(const guVector a, const guVector b)
{
	guVector ab;
	guVecSub(&a,&b,&ab);
	return ab;
}

guVector operator*(const guVector& a, const float& scalar)
{
	return {a.x * scalar, a.y * scalar, a.z * scalar};
}

void operator*=(guVector& a, const float& scalar)
{
	a.x *= scalar;
	a.y *= scalar;
	a.z *= scalar;
}

guVector operator/(const guVector& a, const float& scalar)
{
	if (scalar != 0.0f)
	{
		return {a.x / scalar, a.y / scalar, a.z / scalar};
	}
	else
	{
		return {0,0,0};
	}
}

void operator/=(guVector& a, const float& scalar)
{
	if (scalar != 0.0f)
	{
		a.x /= scalar;
		a.y /= scalar;
		a.z /= scalar;
	}
	else
	{
		a.x = 0.0f;
		a.y = 0.0f;
		a.z = 0.0f;
	}
}

float guVecMagnitude(const guVector &a)
{
	if (guVecIsZero(a))
	{
		return 0.0f;
	}
	return (float)sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

bool guVecIsZero(const guVector& a)
{
	return (a.x == 0.0f && a.y == 0.0f && a.z == 0.0f);
}

guVector guVecLimit(guVector a, float magnitude)
{
	if (guVecIsZero(a))
	{
		return a;
	}
	float m = guVecMagnitude(a);
	if (m > magnitude)
	{
		guVecNormalize(&a);
		a.x *= magnitude;
		a.y *= magnitude;
		a.z *= magnitude;
	}
	return a;
}

void guVecZero ( guVector& a )
{
	a.x = 0.0f;
	a.y = 0.0f;
	a.z = 0.0f;
}

