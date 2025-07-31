#include <mgdl/mgdl-draw3d.h>
#include <mgdl/mgdl-opengl.h>


static void Quad(
    const V3f A_,
    const V3f B_,
    const V3f C_,
    const V3f D_,
    const V3f normal)
{
	// bottom
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(V3f_X(A_), V3f_Y(A_), V3f_Z(A_));

	// bottom2
	glVertex3f(V3f_X(D_), V3f_Y(D_), V3f_Z(D_));

	// top 1 and 2
	glVertex3f(V3f_X(C_), V3f_Y(C_), V3f_Z(C_));

	glVertex3f(V3f_X(B_), V3f_Y(B_), V3f_Z(B_));
}

void Draw3D_Cube(float size)
{
	static V3f R = V3f_Create(1.0f, 0.0f, 0.0f);
	static V3f L = V3f_Create(-1.0f, 0.0f, 0.0f);
	static V3f U = V3f_Create(0.0f, 1.0f, 0.0f);
	static V3f D = V3f_Create(0.0f, -1.0f, 0.0f);
	static V3f F = V3f_Create(0.0f, 0.0f, 1.0f);
	static V3f B = V3f_Create(0.0f, 0.0f, -1.0f);

    const float hs = size/2.0f;
	V3f bl = V3f_Create(-hs, -hs, -hs);
	V3f br = V3f_Create( hs, -hs, -hs);
	V3f fl = V3f_Create(-hs, -hs,  hs);
	V3f fr = V3f_Create( hs, -hs,  hs);

	V3f tbl = V3f_Create(-hs, hs, -hs); // top back left
	V3f tbr = V3f_Create( hs, hs, -hs); // top back right
	V3f tfl = V3f_Create(-hs, hs, hs); //  top forward left
	V3f tfr = V3f_Create( hs, hs, hs);

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
