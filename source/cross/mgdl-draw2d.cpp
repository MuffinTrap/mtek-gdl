#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-util.h>


void gdl::DrawBox(short x, short y, short x2, short y2, u32 color)
{
	glBegin(GL_LINE_LOOP);
		gdl::RGBA8Floats f= gdl::ColorToFloats(color);
		glColor3f(f.red, f.green, f.blue);
		// TOP LEFT
		glVertex2f(x, y);

		// LOW LEFT!
		glVertex2f(x, y2);

		// LOW RIGHT
		glVertex2f(x2, y2);

		// TOP RIGHT
		glVertex2f(x2, y);


	glEnd();
}

void gdl::DrawBoxF(short x, short y, short x2, short y2, u32 color)
{
	gdl::RGBA8Floats f= gdl::ColorToFloats(color);
	glColor3f(f.red, f.green, f.blue);
	glBegin(GL_QUADS);
		// TOP LEFT
		glVertex2f(x, y);

		// LOW LEFT!
		glVertex2f(x, y2);

		// LOW RIGHT
		glVertex2f(x2, y2);

		// TOP RIGHT
		glVertex2f(x2, y);


	glEnd();
}
