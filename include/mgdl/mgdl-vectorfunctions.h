#pragma once
#include <mgdl/mgdl-vector.h>

// Macros to unify ccVector and cglm

	/*
#ifdef MGDL_PLATFORM_WII
	// On the wii the gu library is used

	#define V2f_Create(x, y) {x, y}
	#define V2f_X(Va) Va.x
	#define V2f_Y(Va) Va.y

	#define V3f_Create(x,y,z) {x, y, z}
	#define V3f_X(Va) Va.x
	#define V3f_Y(Va) Va.y
	#define V3f_Z(Va) Va.z

	#define V3f_Copy(source, dest) dest = source;
	#define V3f_FromFloatArray(array, index, dest) V3f_Create(array[index], array[index+1], array[index+2]);
	#define V3f_ToFloatArray(source, array, index) array[index*3] = source.x; array[index*3+1] = source.y; array[index*3+2] = source.z;
	#define V3f_FromVec3Array(array, index, dest) dest = array[index];

	#define V3f_Scale(Va, scalar, dest) guVecScale(&Va, &dest, scalar);
	#define V3f_Add(Va, Vb, dest) guVecAdd(&Va, &Vb, &dest);
	#define V3f_Sub(Va, Vb, dest) guVecSub(&Va, &Vb, &dest);
	#define V3f_Normalize(Va) guVecNormalize(&Va);
	#define V3f_Cross(Va, Vb, dest) guVecCross(&Va, &Vb, &dest);
	#define V3f_Dot(Va, Vb) guVecDotProduct(&Va, &Vb);

		// Rotations
	vec3 V3f_RotateYFunc(vec3 source, float angle);
	#define V3f_RotateY(source, angle, dest) dest = V3f_RotateYFunc(source, angle);

	vec3 V3f_RotateZFunc(vec3 source, float angle);
	#define V3f_RotateZ(source, angle, dest) dest = V3f_RotateZFunc(source, angle);


	vec3 V3f_HexToColorFunc(int red, int green, int blue);
	#define V3f_HexToColor(red, green, blue, dest); dest = V3f_HexToColorFunc(red, green, blue);

	float V3f_BezierFuncF(float s, float P0, float C0, float C1, float P1);
	vec3 V3f_BezierFuncV3(float s, vec3 P0, vec3 C0, vec3 C1, vec3 P1);

	#define V3f_BezierV3(s, P0, C0, C1, P1, dest) dest = V3f_BezierFuncV3(s, P0, C0, C1, P1);

	#define V3f_Print(Va) printf("%.2f, %.2f, %.2f\n", Va.x, Va.y, Va.z);


//////////////////////////////////////////////////////////////////////

#else // PC platform
*/

	#ifdef MGDL_USE_CCVECTOR

	// Vector 2
	#define V2f_Create(x,y) vec2New(x,y)
	#define V2f_X(Va) Va.x
	#define V2f_Y(Va) Va.y

	#define V2f_Add(Va, Vb) vec2Add(Va, Vb)

	// Vector3

	#define V3f_Create(x,y,z) vec3New(x,y,z)
	#define V3f_X(Va) Va.x
	#define V3f_Y(Va) Va.y
	#define V3f_Z(Va) Va.z

	#define V3f_Copy(source, dest) dest = source;
	#define V3f_FromFloatArray(array, index, dest) V3f_Create(array[index], array[index+1], array[index+2]);
	#define V3f_ToFloatArray(source, array, index) array[index*3] = source.x; array[index*3+1] = source.y; array[index*3+2] = source.z;
	#define V3f_FromVec3Array(array, index, dest) dest = array[index];

	#define V3f_Scale(Va, scalar, dest) dest = vec3Multiply(Va, scalar)
	#define V3f_Add(Va, Vb, dest) dest = vec3Add(Va, Vb)
	#define V3f_Sub(Va, Vb, dest) dest = vec3Sub(Va, Vb)
	#define V3f_Normalize(Va) Va = vec3Normalize(Va)
	#define V3f_Cross(Va, Vb, dest) dest = vec3Cross(Va, Vb)
	#define V3f_Dot(Va, Vb) vec3Dot(Va, Vb)
	#define V3f_Length(Va) vec3Length(Va)

		// Rotations
	vec3 V3f_RotateYFunc(vec3 source, float angle);
	#define V3f_RotateY(source, angle, dest) dest = V3f_RotateYFunc(source, angle)

	vec3 V3f_RotateZFunc(vec3 source, float angle);
	#define V3f_RotateZ(source, angle, dest) dest = V3f_RotateZFunc(source, angle)

	vec3 V3f_HexToColorFunc(int red, int green, int blue);
	#define V3f_HexToColor(red, green, blue, dest); dest = V3f_HexToColorFunc(red, green, blue)

	float V3f_BezierFuncF(float s, float P0, float C0, float C1, float P1);
	vec3 V3f_BezierFuncV3(float s, vec3 P0, vec3 C0, vec3 C1, vec3 P1);

	#define V3f_BezierV3(s, P0, C0, C1, P1, dest) dest = V3f_BezierFuncV3(s, P0, C0, C1, P1)

	#define V3f_Print(Va) printf("%.2f, %.2f, %.2f\n", Va.x, Va.y, Va.z)

	// Vector4

	#define V4f_Create(x, y, z, w) vec4New(x, y, z, w)

	#endif

	/////////////////////////////////////////////////////


	#ifdef MGDL_USE_CGLM
	// Use cgml

	#define V2f_Create(x,y) {x, y}
	#define V2f_X(Va) Va[0]
	#define V2f_Y(Va) Va[1]

	#define V3f_Create(x, y, z) {x, y, z}
	#define V3f_X(Va) Va[0]
	#define V3f_Y(Va) Va[1]
	#define V3f_Z(Va) Va[2]

	#define V3f_Copy(source, dest) glm_vec3_copy(source, dest);
	#define V3f_FromFloatArray(array, index, dest) glm_vec3_make(&array[index], dest);
	#define V3f_ToFloatArray(source, array, index) array[index*3] = source[0]; array[index*3+1] = source[1]; array[index*3+2] = source[2];

	#define V3f_FromVec3Array(array, index, dest) V3f_Copy(array[index], dest);

	#define V3f_Scale(Va, scalar, dest) glm_vec3_scale(Va, scalar, dest);
	#define V3f_Add(Va, Vb, dest) glm_vec3_add(Va, Vb, dest);
	#define V3f_Sub(Va, Vb, dest) glm_vec3_sub(Va, Vb, dest);
	#define V3f_Normalize(Va) glm_vec3_normalize(Va);
	#define V3f_Cross(Va, Vb, dest) glm_vec3_cross(Va, Vb, dest);
	#define V3f_Dot(Va, Vb) glm_vec3_dot(Va, Vb);


	void V3f_RotateYFunc(vec3 p, float angle, vec3 out);
	#define V3f_RotateY(source, angle, dest) V3f_RotateYFunc(source, angle, dest);

	void V3f_RotateZFunc(vec3 p, float angle, vec3 out);
	#define V3f_RotateZ(source, angle, dest) V3f_RotateZFunc(source, angle, dest);

	void V3f_HexToColorFunc(int red, int green, int blue, vec3 color_out);
	#define V3f_HexToColor(red, green, blue, dest); V3f_HexToColorFunc(red, green, blue, dest);

	float V3f_BezierFuncF(float s, float P0, float C0, float C1, float P1);

	void V3f_BezierFuncV3(float s, vec3 P0, vec3 C0, vec3 C1, vec3 P1, vec3 out_point);
	#define V3f_BezierV3(s, P0, C0, C1, P1, dest) V3f_BezierFuncV3(s, P0, C0, C1, P1, dest);

	#define V3f_Print(Va) printf("%.2f, %.2f, %.2f\n", Va[0], Va[1], Va[2]);
	#endif

//#endif // PC platform

