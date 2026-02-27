#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-assert.h>


Mesh* Mesh_CreateEmpty(void)
{
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
	mesh->positions = nullptr;
	mesh->indices = nullptr;
	mesh->normals = nullptr;
	mesh->uvs=nullptr;
	mesh->colors = nullptr;

	mesh->indexCount = 0;
	mesh->vertexCount = 0;
	mesh->indexCounter = 0;

	mesh->name = nullptr;
	return mesh;
}

sizetype Mesh_Init (Mesh* mesh, sizetype vertexCount, sizetype indexCount, u32 creationFlags)
{
	mesh->vertexCount = vertexCount;
	mesh->indexCount = indexCount;

	// Reserve space
	int byteCount = 0;
	mesh->indices = new GLushort[indexCount];
	byteCount += indexCount * sizeof(float);

	{
		// 3 floats per position
		sizetype positionFloats = vertexCount * 3;
		mesh->positions = new GLfloat[positionFloats];
		byteCount += positionFloats * sizeof(float);
	}

	if (Flag_IsSet(creationFlags, FlagNormals))
	{
		// 3 floats per normal
		sizetype normalFloats = vertexCount * 3;
		mesh->normals = new GLfloat[normalFloats];
		byteCount += normalFloats * sizeof(float);
	}

	if (Flag_IsSet(creationFlags, FlagUVs))
	{
		// 2 floats per uv
		sizetype uvFloats = vertexCount * 2;
		mesh->uvs = new GLfloat[uvFloats];
		byteCount += uvFloats * sizeof(float);
	}

	if (Flag_IsSet(creationFlags, FlagColors))
	{
		// 3 floats per color
		sizetype colorFloats = vertexCount * 3;
		mesh->colors = new GLfloat[colorFloats];
		byteCount += colorFloats * sizeof(float);
	}

	return byteCount;
}

void Mesh_SetupVertexArrays(Mesh* mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mesh->positions);

	// TODO Only enable this if lights are used
	if (mesh->normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, mesh->normals);
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (mesh->uvs != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, mesh->uvs);
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (mesh->colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, mesh->colors);
	}
	else
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}
	return;
}

void Mesh_DrawElements(Mesh* mesh)
{
	Mesh_SetupVertexArrays(mesh);
	// NOTE OpenGX does not impelement glDrawRangeElements()
	//glDrawRangeElements(GL_TRIANGLES, 0, vertexCount-1, indexCount, GL_UNSIGNED_SHORT, indices);
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_SHORT, mesh->indices);
}

void Mesh_DrawArrays(Mesh* mesh)
{
	Mesh_SetupVertexArrays(mesh);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
}

void Mesh_DrawPoints(Mesh* mesh)
{
	mgdl_assert_print(mesh->indexCount > 0, "No indices on Mesh_DrawLines");
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mesh->positions);
	glDrawElements(GL_POINTS, mesh->indexCount, GL_UNSIGNED_SHORT, mesh->indices);
}

void Mesh_DrawLines(Mesh* mesh)
{
	mgdl_assert_print(mesh->indexCount > 0, "No indices on Mesh_DrawLines");
	glDisableClientState(GL_VERTEX_ARRAY);
	for (GLsizei i = 0; i < mesh->indexCount; i+=3)
	{
		V3f a = Mesh_GetPosition(mesh, i);
		V3f b = Mesh_GetPosition(mesh, i+1);
		V3f c = Mesh_GetPosition(mesh, i+2);
		glBegin(GL_LINE_LOOP);
			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
		glEnd();
	}
}

void Mesh_DrawNormals(Mesh* mesh)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glBegin(GL_LINES);
	for (GLsizei i = 0; i < mesh->indexCount; i++)
	{
		V3f n = Mesh_GetNormal(mesh, i);
		V3f a = Mesh_GetPosition(mesh, i);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(a.x + n.x, a.y + n.y, a.z + n.z);
	}
	glEnd();
}

void Mesh_CalculateMatcapUVs(Mesh* mesh, mat4x4 modelViewMatrix, mat4x4 normalMatrix)
{
	// This calculation happens in screen space

	V3f eye;
	V3f normal; // screen space normalo
	vec4 normal4;
	V3f reflection;
	vec2 R2;
	V3f position;
	vec4 position4;
	vec2 matcapUV;

	// Overwrite UVs
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		position = Mesh_GetPositionFromArray(mesh, i);
		normal = Mesh_GetNormalFromArray(mesh, i);
	}

	const vec2 half = V2f_Create(0.5f, 0.5f);
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		position = Mesh_GetPositionFromArray(mesh, i);
		normal = Mesh_GetNormalFromArray(mesh, i);

		normal4 = vec4New(normal.x, normal.y, normal.z, 0.0f);
		position4 = vec4New(position.x, position.y, position.z, 1.0f);
		MTX4x4_MultiplyVector(modelViewMatrix, position4, position4);
		position = position4.xyz;
		matcapUV = V2f_Create(0.5f, 0.5f);
		if (V3f_Length(position) != 0.0f)
		{
			V3f_Normalize(position, eye);
			MTX4x4_MultiplyVector(normalMatrix, normal4, normal4);
			V3f_FromV4f_xyz(normal4, normal);
			V3f_Normalize(normal, normal);

			V3f_Reflect(normal, eye, reflection);// Reflect eye with normal

			const float rx2 = pow(reflection.x, 2.0f);
			const float ry2 = pow(reflection.y, 2.0f);
			const float rz12 = pow(reflection.z+1, 2.0f);
			const float sqrtR2 = sqrt(rx2 + ry2 + rz12) * 2.0f;
			R2 = reflection.xy;
			V2f_Add( V2f_Create(R2.x/sqrtR2, R2.y/sqrtR2), half, matcapUV);
		}
		Mesh_SetUVToArray(mesh, i, matcapUV);
	}
}

GLushort Mesh_AddPosition(Mesh* mesh, V3f position)
{
	if (mesh->indexCounter < mesh->vertexCount)
	{
		GLushort i = mesh->indexCounter * 3;
		mesh->positions[i+0] = V3f_X(position);
		mesh->positions[i+1] = V3f_Y(position);
		mesh->positions[i+2] = V3f_Z(position);
	}
	GLushort last = mesh->indexCounter;
	mesh->indexCounter += 1;
	return last;
}

void Mesh_AddNormal(Mesh* mesh, V3f normal)
{
	GLushort index = mesh->indexCounter - 1;
	if (index < mesh->vertexCount)
	{
		GLushort i = index * 3;
		mesh->normals[i+0] = V3f_X(normal);
		mesh->normals[i+1] = V3f_Y(normal);
		mesh->normals[i+2] = V3f_Z(normal);
	}
}

void Mesh_AddUV(Mesh* mesh, vec2 uv)
{

	GLushort index = mesh->indexCounter - 1;
	if (index < mesh->vertexCount)
	{
		GLushort i = index * 2;
		mesh->uvs[i+0] = V3f_X(uv);
		mesh->uvs[i+1] = V3f_Y(uv);
	}
}

void Mesh_AddColor(Mesh* mesh, V3f color)
{
	GLushort index = mesh->indexCounter - 1;
	if (index < mesh->vertexCount)
	{
		GLushort i = index * 3;
		mesh->colors[i+0] = V3f_X(color);
		mesh->colors[i+1] = V3f_Y(color);
		mesh->colors[i+2] = V3f_Z(color);
	}
}


u32 Mesh_AddTriangle(Mesh* mesh, GLushort indexA, GLushort indexB, GLushort indexC, u32 index)
{
	mesh->indices[index*3+0] = indexA;
	mesh->indices[index*3+1] = indexB;
	mesh->indices[index*3+2] = indexC;
	return index + 1;
}

// This is a drawing index, not an array index
V3f Mesh_GetPosition ( Mesh* mesh, GLushort index )
{
	if (index < mesh->indexCount)
	{
		// What vertex is drawn when index
		GLushort position = mesh->indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return V3f_Create(mesh->positions[i+0], mesh->positions[i+1], mesh->positions[i+2]);
	}
	Log_ErrorF("No such index! %d > %d\n", index, mesh->indexCount);
	return V3f_Create(0.0f, 0.0f, 0.0f);
}

V3f Mesh_GetNormal (Mesh* mesh, GLushort index )
{
	if (index < mesh->indexCount)
	{
		// What vertex is drawn when index
		GLushort position = mesh->indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return V3f_Create(mesh->normals[i+0], mesh->normals[i+1], mesh->normals[i+2]);
	}
	Log_ErrorF("No such index! %d > %d\n", index, mesh->indexCount);
	return V3f_Create(0.0f, 0.0f, 0.0f);
}


void Mesh_SetNormalToArray ( Mesh* mesh,sizetype index, V3f normal )
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		mesh->normals[vi+0] = V3f_X(normal);
		mesh->normals[vi+1] = V3f_Y(normal);
		mesh->normals[vi+2] = V3f_Z(normal);
	}
}

bool Mesh_GetTriangleIndices (Mesh* mesh, GLsizei triangleIndex, GLushort* outA, GLushort* outB, GLushort* outC )
{
	GLsizei indice = triangleIndex * 3;
	if (indice + 2 < mesh->indexCount)
	{
		*outA = mesh->indices[indice];
		*outB = mesh->indices[indice+1];
		*outC = mesh->indices[indice+2];
		return true;
	}
	return false;
}

void Mesh_SetUVToArray (Mesh* mesh, sizetype index, vec2 uv )
{
	if (mesh->uvs != nullptr)
	{
		sizetype vi = index * 2;
		mesh->uvs[vi + 0] = V2f_X(uv);
		mesh->uvs[vi + 1] = V2f_Y(uv);
	}
}

void Mesh_SetDrawingIndex ( Mesh* mesh, sizetype index, GLushort drawIndex )
{
	mesh->indices[index] = drawIndex;
}


V3f Mesh_GetPositionFromArray(Mesh* mesh,sizetype index)
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		return V3f_Create(mesh->positions[vi+0], mesh->positions[vi+1], mesh->positions[vi+2]);
	}
	Log_ErrorF("index %zu > %u vertexCount!\n", index, mesh->vertexCount);
	return V3f_Create(0.0f, 0.0f, 0.0f);
}
V3f Mesh_GetNormalFromArray(Mesh* mesh,sizetype index)
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		return V3f_Create(mesh->normals[vi+0], mesh->normals[vi+1], mesh->normals[vi+2]);
	}
	Log_ErrorF("index %zu > %u vertexCount!\n", index, mesh->vertexCount);
	return V3f_Create(0.0f, 1.0f, 0.0f);
}

// Mesh creation functions

Mesh* Mesh_CreateIcosahedron(u32 creationFlags)
{
    const float X = 0.525731112119133606;
    const float Z = 0.850650808352039932;
    const float N = 0.0f;

	Mesh* icosa = Mesh_CreateEmpty();
	Mesh_Init(icosa, 12, 60, creationFlags);

	delete[] icosa->positions;
    icosa->positions = new GLfloat[12*3] {
		-X, N, Z,
		 X,  N,  Z,
		-X,  N, -Z,
		 X,  N, -Z,
		 N,  Z,  X,
		 N,  Z, -X,
		 N, -Z,  X,
		 N, -Z, -X,
		 Z,  X,  N,
		-Z,  X,  N,
		 Z, -X,  N,
		-Z, -X,  N};

    if (Flag_IsSet(creationFlags, FlagNormals))
	{
		delete[] icosa->normals;
		icosa->normals = icosa->positions;
	}

    // OpenGL books the indices in CC winding

    delete[] icosa->indices;
	icosa->indices = new GLushort[60] {
		1,4,0,  4,9,0,  4,5,9,  8,5,4, 1,8,4,  // 15
		1,10,8, 10,3,8, 8,3,5, 3,2,5,  3,7,2,  // 30
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,  // 45
		10,1,6, 11,0,9,  2,11,9, 5,2,9, 11,2,7   // 60
		};

	icosa->indexCount = 60;

	icosa->name = "Icosahedron";

    return icosa;
}

void Mesh_DebugPrint(Mesh* mesh)
{
	Log_InfoF("%s mesh has %u vertices and %u indices\n", mesh->name, mesh->vertexCount, mesh->indexCount);
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		sizetype vi = i*3;
		Log_InfoF("%zu: Pos %.2f, %.2f, %.2f\n", i, mesh->positions[vi+0], mesh->positions[vi+1], mesh->positions[vi+2]);
	}
	Log_Info("\n");
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		V3f pos = Mesh_GetPositionFromArray(mesh, i);
		V3f normal = Mesh_GetNormalFromArray(mesh, i);
		Log_InfoF("%zu: Pos %.2f, %.2f, %.2f\tN %.2f, %.2f, %.2f\n", i, pos.x, pos.y, pos.z, normal.x, normal.y, normal.z);
	}
}


Mesh * Mesh_CreateQuad (u32 creationFlags)
{
	Mesh* quad = new Mesh();
	Mesh_Init(quad, 4, 6, creationFlags);
	delete[] quad->positions;

	float sz = 0.5f;
    quad->positions = new GLfloat[4*3] {
		-sz, -sz, 0.0f,
		sz, -sz, 0.0f,
		sz, sz, 0.0f,
		-sz, sz, 0.0f};

	if (Flag_IsSet(creationFlags, FlagNormals))
	{
		for (int i = 0; i < 4*3; i+=3)
		{
			quad->normals[i+0] = 0.0f;
			quad->normals[i+1] = 0.0f;
			quad->normals[i+2] = 1.0f;
		}
	}

	if (Flag_IsSet(creationFlags, FlagUVs))
	{
		Mesh_SetUVToArray(quad, 0, vec2New(0.0f, 0.0f));
		Mesh_SetUVToArray(quad, 1, vec2New(1.0f, 0.0f));
		Mesh_SetUVToArray(quad, 2, vec2New(1.0f, 1.0f));
		Mesh_SetUVToArray(quad, 3, vec2New(0.0f, 1.0f));
	}

	quad->indices[0] = 0;
	quad->indices[1] = 1;
	quad->indices[2] = 2;
	quad->indices[3] = 2;
	quad->indices[4] = 3;
	quad->indices[5] = 0;

	quad->name = "Quad";

	return quad;
}

Mesh* CreateStar(float centerThickness, float pointRadius, float sharpness, int pointAmount, bool bothSides, u32 creationFlags)
{
	Mesh* star = new Mesh();
	Mesh_Init(star, 1 + pointAmount * 3, pointAmount * 6, creationFlags);
	V3f point = V3f_Create(1.0f, 0.0f, 0.0f);
	float pointAngle = M_PI*2/(float)pointAmount;
	float halfAngle = pointAngle/2.0f;
	float baseRadius = pointRadius * (1.0f-sharpness);
	u32 triangleCount = 0;

	int sides = 1;
	if (bothSides)
	{
		sides = 2;
	}
	else
	{
		centerThickness = 0.0f;
	}

	for (int side = 0; side < sides; side++)
	{
		V3f topCenter = V3f_Create(0, centerThickness * -1.0f * side, 0);
		GLushort top_center = Mesh_AddPosition(star, topCenter);
        for (int p = 0; p < pointAmount; p++)
        {
			// Wind triangles the other way on the other side
			// Front side : side == 0
			// Back side  : side == 1
            // Front side is facing Z axis
            V3f baseRot1 ; V3f_RotateZ(point, pointAngle * (p + side), baseRot1);
            V3f baseRot2 ; V3f_RotateZ(point, pointAngle * (p + 1 - side), baseRot2);
            V3f pointRot ; V3f_RotateZ(point, pointAngle * p + halfAngle, pointRot);

            V3f rimPoint   ; V3f_Scale(pointRot, pointRadius, rimPoint);
            V3f basePoint1 ; V3f_Scale(baseRot1 , baseRadius, basePoint1);
            V3f basePoint2 ; V3f_Scale(baseRot2 , baseRadius, basePoint2);

            // V3f normal1 = CalculateTriangleNormal(basePoint1, topCenter, rimPoint);
            GLushort rim =   Mesh_AddPosition(star, rimPoint);
            GLushort base1 = Mesh_AddPosition(star, basePoint1);
            GLushort base2 = Mesh_AddPosition(star, basePoint2);

            triangleCount = Mesh_AddTriangle(star, top_center, rim, base1, triangleCount);
            triangleCount = Mesh_AddTriangle(star, top_center, base2, rim, triangleCount);
        }
	}

	return star;
}

void Mesh_DrawStarBorder(float borderThickness, float pointRadius, float sharpness, int pointAmount)
{
	float ratio = (1.0f - sharpness);
	float baseRadius = pointRadius * ratio;
	//////////////////////////////////////////

	V3f point = V3f_Create(1.0f, 0.0f, 0.0f);
	float fifth = (M_PI*2.0f)/(float)pointAmount;
	float tenth = fifth/2.0f;

	// This is a magic number to make the borders of even thickness
	float fixRatio = 1.0f - sharpness;

	glBegin(GL_TRIANGLES);
	for (int p = 0; p < pointAmount; p++)
	{
		// star is facing Z axis
		V3f baseRot1;  V3f_RotateZ(point, fifth * p, baseRot1);
		V3f baseRot2;  V3f_RotateZ(point, fifth * (p+1), baseRot2);
		V3f pointRot;  V3f_RotateZ(point, fifth * p + tenth, pointRot);

		// Inner points
		V3f rimPointI ;   V3f_Scale(pointRot, pointRadius - borderThickness, rimPointI);
		V3f basePoint1I ; V3f_Scale(baseRot1 , baseRadius - borderThickness * fixRatio, basePoint1I);
		V3f basePoint2I ; V3f_Scale(baseRot2 , baseRadius - borderThickness * fixRatio, basePoint2I);

		// Outer points
		V3f rimPointO ;   V3f_Scale(pointRot, pointRadius, rimPointO);
		V3f basePoint1O ; V3f_Scale(baseRot1 , baseRadius, basePoint1O);
		V3f basePoint2O ; V3f_Scale(baseRot2 , baseRadius, basePoint2O);

		/*
		GLushort rim_in =    Mesh_AddPosition(mesh, rimPointI);
		GLushort base1_in =  Mesh_AddPosition(mesh, basePoint1I);
		GLushort base1_out = Mesh_AddPosition(mesh, basePoint1O);

		GLushort rim_out =   Mesh_AddPosition(mesh, rimPointO);
		GLushort base2_in =  Mesh_AddPosition(mesh, basePoint2I);
		GLushort base2_out = Mesh_AddPosition(mesh, basePoint2O);
		*/

		mgdl_glTriangleV3F_xy(rimPointI, basePoint1I, basePoint1O);
		mgdl_glTriangleV3F_xy(basePoint1O, rimPointO, rimPointI);
		mgdl_glTriangleV3F_xy(rimPointI, basePoint2O, basePoint2I);
		mgdl_glTriangleV3F_xy(basePoint2O, rimPointI, rimPointO);

		/*
		triangleCount = Mesh_AddTriangle(mesh, rim_in, base1_in, base1_out, triangleCount); //3
		triangleCount = Mesh_AddTriangle(mesh, base1_out, rim_out, rim_in, triangleCount); //6
		triangleCount = Mesh_AddTriangle(mesh, rim_in, base2_in, base2_out, triangleCount); //9
		triangleCount = Mesh_AddTriangle(mesh, base2_out, rim_out, rim_in, triangleCount); //12
		*/
	}
	glEnd();
}

Mesh* Mesh_CreateStarBorder(float borderThickness, float pointRadius, float sharpness, int pointAmount, u32 creationFlags)
{
	Mesh* mesh = new Mesh();
	float ratio = (1.0f - sharpness);
	float baseRadius = pointRadius * ratio;
	u32 triangleCount = 0;
	//////////////////////////////////////////

	Mesh_Init(mesh, pointAmount * 6, 12 * pointAmount, creationFlags);

	V3f point = V3f_Create(1.0f, 0.0f, 0.0f);
	float fifth = M_PI*2.0f/(float)pointAmount;
	float tenth = fifth/2.0f;

	// This is a magic number to make the borders of even thickness
	float fixRatio = 0.50f;

	for (int p = 0; p < pointAmount; p++)
	{
		// star is facing Z axis
		V3f baseRot1;  V3f_RotateZ(point, fifth * p, baseRot1);
		V3f baseRot2;  V3f_RotateZ(point, fifth * (p+1), baseRot2);
		V3f pointRot;  V3f_RotateZ(point, fifth * p + tenth, pointRot);

		// Inner points
		V3f rimPointI ;   V3f_Scale(pointRot, pointRadius - borderThickness, rimPointI);
		V3f basePoint1I ; V3f_Scale(baseRot1 , baseRadius - borderThickness * fixRatio, basePoint1I);
		V3f basePoint2I ; V3f_Scale(baseRot2 , baseRadius - borderThickness * fixRatio, basePoint2I);

		// Outer points
		V3f rimPointO ;   V3f_Scale(pointRot, pointRadius, rimPointO);
		V3f basePoint1O ; V3f_Scale(baseRot1 , baseRadius, basePoint1O);
		V3f basePoint2O ; V3f_Scale(baseRot2 , baseRadius, basePoint2O);

		GLushort rim_in =    Mesh_AddPosition(mesh, rimPointI);
		GLushort base1_in =  Mesh_AddPosition(mesh, basePoint1I);
		GLushort base1_out = Mesh_AddPosition(mesh, basePoint1O);

		GLushort rim_out =   Mesh_AddPosition(mesh, rimPointO);
		GLushort base2_in =  Mesh_AddPosition(mesh, basePoint2I);
		GLushort base2_out = Mesh_AddPosition(mesh, basePoint2O);

		triangleCount = Mesh_AddTriangle(mesh, rim_in, base1_in, base1_out, triangleCount); //3
		triangleCount = Mesh_AddTriangle(mesh, base1_out, rim_out, rim_in, triangleCount); //6
		triangleCount = Mesh_AddTriangle(mesh, rim_in, base2_out, base2_in, triangleCount); //9
		triangleCount = Mesh_AddTriangle(mesh, base2_out, rim_in, rim_out, triangleCount); //12
	}


	return mesh;

}



