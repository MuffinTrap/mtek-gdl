#pragma once
#include <mgdl/mgdl-vector.h>

#if defined(MGDL_USE_CCVECTOR)

	// Vector 2
	#define V2f_Create(x,y) vec2New(x,y)
	#define V2f_X(Va) Va.x
	#define V2f_Y(Va) Va.y

	#define V2f_Add(Va, Vb, dest) dest = vec2Add(Va, Vb)
	#define V2f_Sub(Va, Vb, dest) dest = vec2Subtract(Va, Vb)

	// Vector3

	#define V3f_Create(x,y,z) vec3New(x,y,z)
	#define V3f_FromV4f_xyz(Vsource, Vdest) Vdest = Vsource.xyz
	#define V3f_X(Va) Va.x
	#define V3f_Y(Va) Va.y
	#define V3f_Z(Va) Va.z

	#define V3f_Copy(source, dest) dest = source;
	#define V3f_FromFloatArray(array, index, dest) V3f_Create(array[index], array[index+1], array[index+2]);
	#define V3f_ToFloatArray(source, array, index) array[index*3] = source.x; array[index*3+1] = source.y; array[index*3+2] = source.z;
	#define V3f_FromVec3Array(array, index, dest) dest = array[index];

	#define V3f_Scale(Va, scalar, dest) dest = vec3Multiply(Va, scalar)
	#define V3f_Add(Va, Vb, dest) dest = vec3Add(Va, Vb)
	#define V3f_Sub(Va, Vb, dest) dest = vec3Subtract(Va, Vb)
	#define V3f_Normalize(Va, dest) dest = vec3Normalize(Va)
	#define V3f_Cross(Va, Vb, dest) dest = vec3Cross(Va, Vb)
	#define V3f_Dot(Va, Vb) vec3Dot(Va, Vb)
	#define V3f_Length(Va) vec3Length(Va)
	#define V3f_Reflect(Vnormal, Veye, Vdest) Vdest = vec3Reflect(Vnormal, Veye)

		// Rotations
	V3f V3f_RotateYFunc(V3f source, float angle);
	#define V3f_RotateY(source, angle, dest) dest = V3f_RotateYFunc(source, angle)

	V3f V3f_RotateZFunc(V3f source, float angle);
	#define V3f_RotateZ(source, angle, dest) dest = V3f_RotateZFunc(source, angle)

	V3f V3f_HexToColorFunc(int red, int green, int blue);
	#define V3f_HexToColor(red, green, blue, dest); dest = V3f_HexToColorFunc(red, green, blue)

	float V3f_BezierFuncF(float s, float P0, float C0, float C1, float P1);
	V3f V3f_BezierFuncV3(float s, V3f P0, V3f C0, V3f C1, V3f P1);

	#define V3f_BezierV3(s, P0, C0, C1, P1, dest) dest = V3f_BezierFuncV3(s, P0, C0, C1, P1)

	#define V3f_Print(Va) printf("%.2f, %.2f, %.2f\n", Va.x, Va.y, Va.z)

	// Vector4

	#define V4f_Create(x, y, z, w) vec4New(x, y, z, w)

	// Matrix 3x3
	#define MTX3x3_Identity(matrix) mat3x3Identity(matrix);
	#define MTX3x3_MultiplyVector(matrix, source, dest) dest = mat3x3MultiplyVector(matrix, source);
	#define MTX3x3_RotateX(matrix, radians) mat3x3RotateX(matrix, (radians))
	#define MTX3x3_RotateY(matrix, radians) mat3x3RotateY(matrix, (radians))
	#define MTX3x3_RotateZ(matrix, radians) mat3x3RotateZ(matrix, (radians))

	// Matrix 4x4
	#define MTX4x4_MultiplyVector(matrix, source, dest) dest = mat4x4MultiplyVector(matrix, source)

#elif defined(MGDL_USE_CGLM)
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


	void V3f_RotateYFunc(V3f p, float angle, V3f out);
	#define V3f_RotateY(source, angle, dest) V3f_RotateYFunc(source, angle, dest);

	void V3f_RotateZFunc(V3f p, float angle, V3f out);
	#define V3f_RotateZ(source, angle, dest) V3f_RotateZFunc(source, angle, dest);

	void V3f_HexToColorFunc(int red, int green, int blue, V3f color_out);
	#define V3f_HexToColor(red, green, blue, dest); V3f_HexToColorFunc(red, green, blue, dest);

	float V3f_BezierFuncF(float s, float P0, float C0, float C1, float P1);

	void V3f_BezierFuncV3(float s, V3f P0, V3f C0, V3f C1, V3f P1, V3f out_point);
	#define V3f_BezierV3(s, P0, C0, C1, P1, dest) V3f_BezierFuncV3(s, P0, C0, C1, P1, dest);

	#define V3f_Print(Va) printf("%.2f, %.2f, %.2f\n", Va[0], Va[1], Va[2]);

#elif defined(MGDL_PLATFORM_WII)
	// On the wii the gu library is used if not CC_VECTOR or CGML

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
	V3f V3f_RotateYFunc(V3f source, float angle);
	#define V3f_RotateY(source, angle, dest) dest = V3f_RotateYFunc(source, angle);

	V3f V3f_RotateZFunc(V3f source, float angle);
	#define V3f_RotateZ(source, angle, dest) dest = V3f_RotateZFunc(source, angle);


	V3f V3f_HexToColorFunc(int red, int green, int blue);
	#define V3f_HexToColor(red, green, blue, dest); dest = V3f_HexToColorFunc(red, green, blue);

	float V3f_BezierFuncF(float s, float P0, float C0, float C1, float P1);
	V3f V3f_BezierFuncV3(float s, V3f P0, V3f C0, V3f C1, V3f P1);

	#define V3f_BezierV3(s, P0, C0, C1, P1, dest) dest = V3f_BezierFuncV3(s, P0, C0, C1, P1);

	#define V3f_Print(Va) printf("%.2f, %.2f, %.2f\n", Va.x, Va.y, Va.z);


	// Matrix 3x3
	#define MTX3x3_Identity(matrix) guMtxIdentity(matrix);
	#define MTX3x3_MultiplyVector(matrix, Va, dest) guVecMultiply(matrix, &Va, &dest);
	// TODO test these, what is the correct letter
	#define MTX3x3_RotateX(matrix, radians) guMtxRotRad(matrix, "x", (radians));
	#define MTX3x3_RotateY(matrix, radians) guMtxRotRad(matrix, "y", (radians));
	#define MTX3x3_RotateZ(matrix, radians) guMtxRotRad(matrix, "z", (radians));

#endif

