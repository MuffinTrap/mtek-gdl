#pragma once
#include <ogc/gu.h>
#include <math.h>

// Operator overloading for guVector

guVector operator+(const guVector a, const guVector b);


guVector operator-(const guVector a, const guVector b);

guVector operator*(const guVector& a, const float& scalar);

void operator*=(guVector& a, const float& scalar);

guVector operator/(const guVector& a, const float& scalar);

void operator/=(guVector& a, const float& scalar);

float guVecMagnitude(const guVector &a);

guVector guVecLimit(guVector a, float magnitude);
void guVecZero(guVector& a);
bool guVecIsZero(const guVector& a);
