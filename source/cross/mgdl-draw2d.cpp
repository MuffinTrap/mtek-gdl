#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-defaultfont.h>
#include <mgdl/mgdl-config.h>
#include <mgdl/mgdl-platform.h>

Rect Rect_Create(short x, short y, short w, short h)
{
	return {x, y, w, h};
}

Rect Rect_CreateV2f(vec2 position, vec2 size)
{
	return {(short)V2f_X(position), (short)V2f_Y(position), (short)V2f_X(size), (short)V2f_Y(size)};
}

void Draw2D_OrigoTo(short x, short y)
{
	glTranslatef(x, y, 0.0f);
}

void Draw2D_OrigoToV2f(vec2 origo)
{
	glTranslatef((short)V2f_X(origo), (short)V2f_Y(origo), 0.0f);
}

void Draw2D_RectLines(short x, short y, short x2, short y2, Color4f* color)
{
	glBegin(GL_LINE_LOOP);
		mgdl_glColor4f(color);
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

void Draw2D_RectLinesWH(short x, short y, short w, short h, Color4f* color)
{
	Draw2D_RectLines(x, y, x + w, y-h, color);
}

void Draw2D_RectLinesRec(Rect rect, Color4f* color)
{
	Draw2D_RectLines(rect.x, rect.y, rect.x + rect.w, rect.y - rect.h, color);
}

void Draw2D_Rect(short x, short y, short x2, short y2, Color4f* color)
{
	mgdl_glColor4f(color);
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

void Draw2D_RectWH(short x, short y, short w, short h, Color4f* color)
{
	Draw2D_Rect(x, y, x + w, y-h, color);
}

void Draw2D_RectR(Rect rect, Color4f* color)
{
	Draw2D_Rect(rect.x, rect.y, rect.x + rect.w, rect.y - rect.h, color);
}

void Draw2D_Quad ( short xtl, short ytl, short xbl, short ybl, short xbr, short ybr, short xtr, short ytr, Color4f* color )
{
	mgdl_glColor4f(color);
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

void Draw2D_QuadV2f(vec2 tl, vec2 bl, vec2 br, vec2 tr, Color4f* color)
{
	Draw2D_Quad(
		(short)V2f_X(tl), (short)V2f_Y(tl),
		(short)V2f_X(bl), (short)V2f_Y(bl),
		(short)V2f_X(br), (short)V2f_Y(br),
		(short)V2f_X(tr), (short)V2f_Y(tr),
		color);
}

void Draw2D_Line ( short x, short y, short x2, short y2, Color4f* color )
{
	mgdl_glColor4f(color);
	glBegin(GL_LINES);
		// TOP LEFT
		glVertex2f(x, y);
		glVertex2f(x2, y2);

		// LOW LEFT!
	glEnd();
}

void Draw2D_LineV2f(vec2 start, vec2 end, Color4f* color )
{
	Draw2D_Line((short)V2f_X(start), (short)V2f_Y(start),
				(short)V2f_X(end), (short)V2f_Y(end), color);
}

void Draw2D_Text(short x, short y, const char* text, Color4f* color)
{
	Font* db = DefaultFont_GetDefaultFont();
	Font_Print(db, color, x, y, db->characterHeight, text);
}

void Draw2D_TextV2f(vec2 position, const char* text, Color4f* color)
{
	Draw2D_Text((short)V2f_X(position), (short)V2f_Y(position), text, color);
}
