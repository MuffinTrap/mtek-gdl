#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-defaultfont.h>
#include <mgdl/mgdl-config.h>
#include <mgdl/mgdl-platform.h>

inline static void OpenGLRect(float x, float y, float x2, float y2, Color4f* color)
{
	mgdl_glColor4f(color);
	// TOP LEFT
	glVertex2f(x, y);

	// LOW LEFT!
	glVertex2f(x, y2);

	// LOW RIGHT
	glVertex2f(x2, y2);

	// TOP RIGHT
	glVertex2f(x2, y);
}

void mgdl_DrawRectangle(float x, float y, float w, float h, Color4f* color)
{
	glBegin(GL_QUADS);
		OpenGLRect(x,y,x+w,y-h,color);
	glEnd();
}
void mgdl_DrawRectangleV(vec2 topleft, vec2 size, Color4f* color)
{
	glBegin(GL_QUADS);
		OpenGLRect(topleft.x, topleft.y, topleft.x+size.x, topleft.y-size.y, color);
	glEnd();
}

void mgdl_DrawRectangleLines(float x, float y, float x2, float y2, Color4f* color)
{
	glBegin(GL_LINE_LOOP);
		OpenGLRect(x,y,x2,y2,color);
	glEnd();
}

void mgdl_DrawRectangleLinesEx(RectF rect, float lineThickness, Color4f* color)
{
	glLineWidth(lineThickness);
	glBegin(GL_LINE_LOOP);
		OpenGLRect(rect.x, rect.y, rect.x+rect.w, rect.y-rect.h, color);
	glEnd();
	glLineWidth(1.0f);
}

void mgdl_DrawLine ( float x, float y, float x2, float y2, Color4f* color )
{
	glBegin(GL_LINES);
		mgdl_glColor4f(color);
		glVertex2f(x, y);
		glVertex2f(x2, y2);
	glEnd();
}

void mgdl_DrawLineV(vec2 start, vec2 end, Color4f* color )
{
	glBegin(GL_LINES);
		mgdl_glColor4f(color);
		glVertex2f(start.x, start.y);
		glVertex2f(end.x, end.y);
	glEnd();
}

void mgdl_DrawText(float x, float y, const char* text, Color4f* color)
{
	Texture* db = DefaultFont_GetDefaultFont();
	Texture_DrawText(db, color, x, y, Texture_GetCharacterHeight(db), text);
}

void Draw2D_TextV(vec2 position, const char* text, Color4f* color)
{
	mgdl_DrawText(V2f_X(position), V2f_Y(position), text, color);
}
