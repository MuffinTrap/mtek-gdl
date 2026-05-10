
#include <mgdl/mgdl-vectorfunctions.h>

Vector2 Vector2New(float px, float py)
{
	Vector2 v;
	v.x = px;
	v.y = py;
	return v;
}

Vector3 Vector3New(float px, float py, float pz)
{
	Vector3 v;
	v.x = px;
	v.y = py;
	v.z = pz;
	return v;
}
Vector4 Vector4New(float px, float py, float pz, float pw)
{
	Vector4 v;
	v.x = px;
	v.y = py;
	v.z = pz;
	v.w = pw;
	return v;

}

Vector3 Vector3RotateY(Vector3 p, float angle) {
	float xt = p.x*cos(angle) - p.z*sin(angle);
	float yt = p.y;
	float zt = p.x*sin(angle) + p.z*cos(angle);
	return Vector3New(xt, yt, zt);
}

Vector3 Vector3RotateZ(Vector3 p, float angle) {
	float xt = p.x*cos(angle) - p.y*sin(angle);
	float yt = p.x*sin(angle) + p.y*cos(angle);
	float zt = p.z;
	return Vector3New(xt, yt, zt);
}

Vector3 Vector3HexToColorFunc(int rx, int gx, int bx)
{
	float r = (float)rx/255.0f;
	float g = (float)gx/255.0f;
	float b = (float)bx/255.0f;
	return Vector3New(r,g,b);
}

float Vector3_BezierFuncF( float s, float P0, float C0, float C1, float P1)
{
	return ( pow(P0*(1-s),3) + pow(3*C0*s*(1-s),2) + pow(3*C1*s,2)*(1-s) + pow(P1*s,3));
}

Vector3 Vector3_BezierFuncV3(float s, Vector3 P0, Vector3 C0, Vector3 C1, Vector3 P1)
{
	float currentX = Vector3_BezierFuncF(s, Vector3_X(P0), Vector3_X(C0), Vector3_X(C1), Vector3_X(P1));
	float currentY = Vector3_BezierFuncF(s, Vector3_Y(P0), Vector3_Y(C0), Vector3_Y(C1), Vector3_Y(P1));
	float currentZ = Vector3_BezierFuncF(s, Vector3_Z(P0), Vector3_Z(C0), Vector3_Z(C1), Vector3_Z(P1));

	Vector3 current = Vector3New(currentX, currentY, currentZ);
	return current;
}

Matrix MatrixFromGL(GLfloat mtx[16])
{
	Matrix m;
	m.m0 = mtx[0];
	m.m1 = mtx[1];
	m.m2 = mtx[2];
	m.m3 = mtx[3];

	m.m4 = mtx[4];
	m.m5 = mtx[5];
	m.m6 = mtx[6];
	m.m7 = mtx[7];

	m.m8 = mtx[8];
	m.m9 = mtx[9];
	m.m10 = mtx[10];
	m.m11 = mtx[11];

	m.m12 = mtx[12];
	m.m13 = mtx[13];
	m.m14 = mtx[14];
	m.m15 = mtx[15];
	return m;
}
