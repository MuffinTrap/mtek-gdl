#include <mgdl/mgdl-draw3d.h>
#include <mgdl/mgdl-opengl.h>


static void Quad(
    const Vector3 A_,
    const Vector3 B_,
    const Vector3 C_,
    const Vector3 D_,
    const Vector3 normal)
{
	// bottom
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(Vector3_X(A_), Vector3_Y(A_), Vector3_Z(A_));

	// bottom2
	glVertex3f(Vector3_X(D_), Vector3_Y(D_), Vector3_Z(D_));

	// top 1 and 2
	glVertex3f(Vector3_X(C_), Vector3_Y(C_), Vector3_Z(C_));

	glVertex3f(Vector3_X(B_), Vector3_Y(B_), Vector3_Z(B_));
}

void Draw3D_Cube(float size)
{
	static Vector3 R = Vector3New(1.0f, 0.0f, 0.0f);
	static Vector3 L = Vector3New(-1.0f, 0.0f, 0.0f);
	static Vector3 U = Vector3New(0.0f, 1.0f, 0.0f);
	static Vector3 D = Vector3New(0.0f, -1.0f, 0.0f);
	static Vector3 F = Vector3New(0.0f, 0.0f, 1.0f);
	static Vector3 B = Vector3New(0.0f, 0.0f, -1.0f);

    const float hs = size/2.0f;
	Vector3 bl = Vector3New(-hs, -hs, -hs);
	Vector3 br = Vector3New( hs, -hs, -hs);
	Vector3 fl = Vector3New(-hs, -hs,  hs);
	Vector3 fr = Vector3New( hs, -hs,  hs);

	Vector3 tbl = Vector3New(-hs, hs, -hs); // top back left
	Vector3 tbr = Vector3New( hs, hs, -hs); // top back right
	Vector3 tfl = Vector3New(-hs, hs, hs); //  top forward left
	Vector3 tfr = Vector3New( hs, hs, hs);

	glBegin(GL_QUADS);
		// left side
		Quad(bl, tbl, tfl, fl, L);
		// right
		Quad(fr, tfr, tbr, br, R);

		// front
		Quad(fl, tfl, tfr, fr, F);
		// back
		 Quad(br, tbr, tbl, bl, B);

         // top
        Quad(tfl, tbl, tbr, tfr, U);
		 // bottom
		Quad(fr, br, bl, fl , D);
	glEnd();
}
