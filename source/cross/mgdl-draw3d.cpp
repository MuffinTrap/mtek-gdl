#include <mgdl/mgdl-draw3d.h>
#include <mgdl/mgdl-opengl.h>


static void Quad(
    const vec3 A_,
    const vec3 B_,
    const vec3 C_,
    const vec3 D_,
    const vec3 normal)
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
	static vec3 R = V3f_Create(1.0f, 0.0f, 0.0f);
	static vec3 L = V3f_Create(-1.0f, 0.0f, 0.0f);
	static vec3 U = V3f_Create(0.0f, 1.0f, 0.0f);
	static vec3 D = V3f_Create(0.0f, -1.0f, 0.0f);
	static vec3 F = V3f_Create(0.0f, 0.0f, 1.0f);
	static vec3 B = V3f_Create(0.0f, 0.0f, -1.0f);

    const float hs = size/2.0f;
	vec3 bl = V3f_Create(-hs, -hs, -hs);
	vec3 br = V3f_Create( hs, -hs, -hs);
	vec3 fl = V3f_Create(-hs, -hs,  hs);
	vec3 fr = V3f_Create( hs, -hs,  hs);

	vec3 tbl = V3f_Create(-hs, hs, -hs); // top back left
	vec3 tbr = V3f_Create( hs, hs, -hs); // top back right
	vec3 tfl = V3f_Create(-hs, hs, hs); //  top forward left
	vec3 tfr = V3f_Create( hs, hs, hs);

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
