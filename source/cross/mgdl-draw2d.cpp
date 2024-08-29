#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-util.h>


void gdl::DrawBox(short x, short y, short x2, short y2, u32 color)
{
	glBegin(GL_LINE_LOOP);
		gdl::RGBA8Floats f= gdl::ColorToFloats(color);
		glColor3f(f.red, f.green, f.blue);
		// TOP LEFT
		glVertex3f(x, y, 0.0f);

		// TOP RIGHT
		glVertex3f(x2, y, 0.0f);

		// LOW RIGHT
		glVertex3f(x2, y2, 0.0f);

		// LOW LEFT!
		glVertex3f(x, y2, 0.0f);
	glEnd();
}

void gdl::DrawBoxF(short x, short y, short x2, short y2, u32 color)
{
	glBegin(GL_QUADS);
		gdl::RGBA8Floats f= gdl::ColorToFloats(color);
		glColor3f(f.red, f.green, f.blue);
		// TOP LEFT
		glVertex3f(x, y, 0.0f);

		// TOP RIGHT
		glVertex3f(x2, y, 0.0f);

		// LOW RIGHT
		glVertex3f(x2, y2, 0.0f);

		// LOW LEFT!
		glVertex3f(x, y2, 0.0f);
	glEnd();
}
