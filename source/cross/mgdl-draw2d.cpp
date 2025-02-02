#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-debugfont.h>
#include <mgdl/mgdl-config.h>
#include <mgdl/mgdl-platform.h>


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

void gdl::DrawQuad ( short xtl, short ytl, short xbl, short ybl, short xbr, short ybr, short xtr, short ytr, u32 color )
{
	gdl::RGBA8Floats f= gdl::ColorToFloats(color);
	glColor3f(f.red, f.green, f.blue);
	glBegin(GL_QUADS);
		// TOP LEFT
		glVertex2f(xtl, ytl);

		// LOW LEFT!
		glVertex2f(xbl, ybl);

		// LOW RIGHT
		glVertex2f(xbr, ybr);

		// TOP RIGHT
		glVertex2f(xtr, ytr);
	glEnd();
}

void gdl::DrawLine ( short x, short y, short x2, short y2, u32 color )
{
	gdl::RGBA8Floats f= gdl::ColorToFloats(color);
	glColor3f(f.red, f.green, f.blue);
	glBegin(GL_LINES);
		// TOP LEFT
		glVertex2f(x, y);
		glVertex2f(x2, y2);

		// LOW LEFT!
	glEnd();

}



