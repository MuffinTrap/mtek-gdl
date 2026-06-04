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

	mesh->name = zstr_init();
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

	if (Flag_IsSetAny(creationFlags, FlagNormals))
	{
		// 3 floats per normal
		sizetype normalFloats = vertexCount * 3;
		mesh->normals = new GLfloat[normalFloats];
		byteCount += normalFloats * sizeof(float);
	}

	if (Flag_IsSetAny(creationFlags, FlagUVs))
	{
		// 2 floats per uv
		sizetype uvFloats = vertexCount * 2;
		mesh->uvs = new GLfloat[uvFloats];
		byteCount += uvFloats * sizeof(float);
	}

	if (Flag_IsSetAny(creationFlags, FlagColors))
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
		Vector3 a = Mesh_GetPosition(mesh, i);
		Vector3 b = Mesh_GetPosition(mesh, i+1);
		Vector3 c = Mesh_GetPosition(mesh, i+2);
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
		Vector3 n = Mesh_GetNormal(mesh, i);
		Vector3 a = Mesh_GetPosition(mesh, i);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(a.x + n.x, a.y + n.y, a.z + n.z);
	}
	glEnd();
}

void Mesh_CalculateMatcapUVs(Mesh* mesh, Matrix modelViewMatrix, Matrix normalMatrix)
{
	// This calculation happens in screen space

	Vector3 eye;
	Vector3 normal; // screen space normalo
	Vector4 normal4;
	Vector3 reflection;
	Vector2 R2;
	Vector3 position;
	Vector4 position4;
	Vector2 matcapUV;

	// Overwrite UVs
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		position = Mesh_GetPositionFromArray(mesh, i);
		normal = Mesh_GetNormalFromArray(mesh, i);
	}

	const Vector2 half = Vector2New(0.5f, 0.5f);
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		position = Mesh_GetPositionFromArray(mesh, i);
		normal = Mesh_GetNormalFromArray(mesh, i);

		normal4 = Vector4New(normal.x, normal.y, normal.z, 0.0f);
		position4 = Vector4New(position.x, position.y, position.z, 1.0f);

		position = Vector3Transform(position, modelViewMatrix);

		matcapUV = Vector2New(0.5f, 0.5f);
		if (Vector3Length(position) != 0.0f)
		{
			eye = Vector3Normalize(position);
			normal = Vector3Transform(normal, normalMatrix);
			normal = Vector3Normalize(normal);

			reflection = Vector3Reflect(normal, eye);// Reflect eye with normal

			const float rx2 = pow(reflection.x, 2.0f);
			const float ry2 = pow(reflection.y, 2.0f);
			const float rz12 = pow(reflection.z+1, 2.0f);
			const float sqrtR2 = sqrt(rx2 + ry2 + rz12) * 2.0f;
			R2 = Vector2New(reflection.x, reflection.y);
			matcapUV = Vector2Add( Vector2New(R2.x/sqrtR2, R2.y/sqrtR2), half);
		}
		Mesh_SetUVToArray(mesh, i, matcapUV);
	}
}

GLushort Mesh_AddPosition(Mesh* mesh, Vector3 position)
{
	if (mesh->indexCounter < mesh->vertexCount)
	{
		GLushort i = mesh->indexCounter * 3;
		mesh->positions[i+0] = Vector3_X(position);
		mesh->positions[i+1] = Vector3_Y(position);
		mesh->positions[i+2] = Vector3_Z(position);
	}
	GLushort last = mesh->indexCounter;
	mesh->indexCounter += 1;
	return last;
}

void Mesh_AddNormal(Mesh* mesh, Vector3 normal)
{
	GLushort index = mesh->indexCounter - 1;
	if (index < mesh->vertexCount)
	{
		GLushort i = index * 3;
		mesh->normals[i+0] = Vector3_X(normal);
		mesh->normals[i+1] = Vector3_Y(normal);
		mesh->normals[i+2] = Vector3_Z(normal);
	}
}

void Mesh_AddUV(Mesh* mesh, Vector2 uv)
{

	GLushort index = mesh->indexCounter - 1;
	if (index < mesh->vertexCount)
	{
		GLushort i = index * 2;
		mesh->uvs[i+0] = Vector3_X(uv);
		mesh->uvs[i+1] = Vector3_Y(uv);
	}
}

void Mesh_AddColor(Mesh* mesh, Vector3 color)
{
	GLushort index = mesh->indexCounter - 1;
	if (index < mesh->vertexCount)
	{
		GLushort i = index * 3;
		mesh->colors[i+0] = Vector3_X(color);
		mesh->colors[i+1] = Vector3_Y(color);
		mesh->colors[i+2] = Vector3_Z(color);
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
Vector3 Mesh_GetPosition ( Mesh* mesh, GLushort index )
{
	if (index < mesh->indexCount)
	{
		// What vertex is drawn when index
		GLushort position = mesh->indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return Vector3New(mesh->positions[i+0], mesh->positions[i+1], mesh->positions[i+2]);
	}
	Log_ErrorF("No such index! %d > %d\n", index, mesh->indexCount);
	return Vector3New(0.0f, 0.0f, 0.0f);
}

Vector3 Mesh_GetNormal (Mesh* mesh, GLushort index )
{
	if (index < mesh->indexCount)
	{
		// What vertex is drawn when index
		GLushort position = mesh->indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return Vector3New(mesh->normals[i+0], mesh->normals[i+1], mesh->normals[i+2]);
	}
	Log_ErrorF("No such index! %d > %d\n", index, mesh->indexCount);
	return Vector3New(0.0f, 0.0f, 0.0f);
}


void Mesh_SetNormalToArray ( Mesh* mesh,sizetype index, Vector3 normal )
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		mesh->normals[vi+0] = Vector3_X(normal);
		mesh->normals[vi+1] = Vector3_Y(normal);
		mesh->normals[vi+2] = Vector3_Z(normal);
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

void Mesh_SetUVToArray (Mesh* mesh, sizetype index, Vector2 uv )
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


Vector3 Mesh_GetPositionFromArray(Mesh* mesh,sizetype index)
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		return Vector3New(mesh->positions[vi+0], mesh->positions[vi+1], mesh->positions[vi+2]);
	}
	Log_ErrorF("index %zu > %u vertexCount!\n", index, mesh->vertexCount);
	return Vector3New(0.0f, 0.0f, 0.0f);
}
Vector3 Mesh_GetNormalFromArray(Mesh* mesh,sizetype index)
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		return Vector3New(mesh->normals[vi+0], mesh->normals[vi+1], mesh->normals[vi+2]);
	}
	Log_ErrorF("index %zu > %u vertexCount!\n", index, mesh->vertexCount);
	return Vector3New(0.0f, 1.0f, 0.0f);
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

    if (Flag_IsSetAny(creationFlags, FlagNormals))
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

	icosa->name = zstr_from("Icosahedron");

    return icosa;
}

void Mesh_DebugPrint(Mesh* mesh)
{
	Log_InfoF("%s mesh has %u vertices and %u indices\n", zstr_cstr(&mesh->name), mesh->vertexCount, mesh->indexCount);
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		sizetype vi = i*3;
		Log_InfoF("%zu: Pos %.2f, %.2f, %.2f\n", i, mesh->positions[vi+0], mesh->positions[vi+1], mesh->positions[vi+2]);
	}
	Log_Info("\n");
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		Vector3 pos = Mesh_GetPositionFromArray(mesh, i);
		Vector3 normal = Mesh_GetNormalFromArray(mesh, i);
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

	if (Flag_IsSetAny(creationFlags, FlagNormals))
	{
		for (int i = 0; i < 4*3; i+=3)
		{
			quad->normals[i+0] = 0.0f;
			quad->normals[i+1] = 0.0f;
			quad->normals[i+2] = 1.0f;
		}
	}

	if (Flag_IsSetAny(creationFlags, FlagUVs))
	{
		Mesh_SetUVToArray(quad, 0, Vector2New(0.0f, 0.0f));
		Mesh_SetUVToArray(quad, 1, Vector2New(1.0f, 0.0f));
		Mesh_SetUVToArray(quad, 2, Vector2New(1.0f, 1.0f));
		Mesh_SetUVToArray(quad, 3, Vector2New(0.0f, 1.0f));
	}

	quad->indices[0] = 0;
	quad->indices[1] = 1;
	quad->indices[2] = 2;
	quad->indices[3] = 2;
	quad->indices[4] = 3;
	quad->indices[5] = 0;

	quad->name = zstr_from("Quad");

	return quad;
}

Mesh* CreateStar(float centerThickness, float pointRadius, float sharpness, int pointAmount, bool bothSides, u32 creationFlags)
{
	Mesh* star = new Mesh();
	Mesh_Init(star, 1 + pointAmount * 3, pointAmount * 6, creationFlags);
	Vector3 point = Vector3New(1.0f, 0.0f, 0.0f);
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
		Vector3 topCenter = Vector3New(0, centerThickness * -1.0f * side, 0);
		GLushort top_center = Mesh_AddPosition(star, topCenter);
        for (int p = 0; p < pointAmount; p++)
        {
			// Wind triangles the other way on the other side
			// Front side : side == 0
			// Back side  : side == 1
            // Front side is facing Z axis
            Vector3 baseRot1 = Vector3RotateZ(point, pointAngle * (p + side));
            Vector3 baseRot2 = Vector3RotateZ(point, pointAngle * (p + 1 - side));
            Vector3 pointRot = Vector3RotateZ(point, pointAngle * p + halfAngle);

            Vector3 rimPoint   = Vector3Scale(pointRot, pointRadius);
            Vector3 basePoint1 = Vector3Scale(baseRot1 , baseRadius);
            Vector3 basePoint2 = Vector3Scale(baseRot2 , baseRadius);

            // Vector3 normal1 = CalculateTriangleNormal(basePoint1, topCenter, rimPoint);
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

	Vector3 point = Vector3New(1.0f, 0.0f, 0.0f);
	float fifth = (M_PI*2.0f)/(float)pointAmount;
	float tenth = fifth/2.0f;

	// This is a magic number to make the borders of even thickness
	float fixRatio = 1.0f - sharpness;

	glBegin(GL_TRIANGLES);
	for (int p = 0; p < pointAmount; p++)
	{
		// star is facing Z axis
		Vector3 baseRot1=  Vector3RotateZ(point, fifth * p);
		Vector3 baseRot2=  Vector3RotateZ(point, fifth * (p+1));
		Vector3 pointRot=  Vector3RotateZ(point, fifth * p + tenth);

		// Inner points
		Vector3 rimPointI =   Vector3Scale(pointRot, pointRadius - borderThickness);
		Vector3 basePoint1I = Vector3Scale(baseRot1 , baseRadius - borderThickness * fixRatio);
		Vector3 basePoint2I = Vector3Scale(baseRot2 , baseRadius - borderThickness * fixRatio);

		// Outer points
		Vector3 rimPointO =   Vector3Scale(pointRot, pointRadius);
		Vector3 basePoint1O = Vector3Scale(baseRot1 , baseRadius);
		Vector3 basePoint2O = Vector3Scale(baseRot2 , baseRadius);

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

	Vector3 point = Vector3New(1.0f, 0.0f, 0.0f);
	float fifth = M_PI*2.0f/(float)pointAmount;
	float tenth = fifth/2.0f;

	// This is a magic number to make the borders of even thickness
	float fixRatio = 0.50f;

	for (int p = 0; p < pointAmount; p++)
	{
		// star is facing Z axis
		Vector3 baseRot1=  Vector3RotateZ(point, fifth * p);
		Vector3 baseRot2=  Vector3RotateZ(point, fifth * (p+1));
		Vector3 pointRot=  Vector3RotateZ(point, fifth * p + tenth);

		// Inner points
		Vector3 rimPointI =   Vector3Scale(pointRot, pointRadius - borderThickness);
		Vector3 basePoint1I = Vector3Scale(baseRot1 , baseRadius - borderThickness * fixRatio);
		Vector3 basePoint2I = Vector3Scale(baseRot2 , baseRadius - borderThickness * fixRatio);

		// Outer points
		Vector3 rimPointO =   Vector3Scale(pointRot, pointRadius);
		Vector3 basePoint1O = Vector3Scale(baseRot1 , baseRadius);
		Vector3 basePoint2O = Vector3Scale(baseRot2 , baseRadius);

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



