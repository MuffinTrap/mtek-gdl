#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-defaultfont.h>
#include <mgdl/mgdl-config.h>
#include <mgdl/mgdl-platform.h>

inline static void OpenGLRect(float x, float y, float x2, float y2, color32 color)
{
	mgdl_glColor32(color);
	// TOP LEFT
	glVertex2f(x, y);

	// LOW LEFT!
	glVertex2f(x, y2);

	// LOW RIGHT
	glVertex2f(x2, y2);

	// TOP RIGHT
	glVertex2f(x2, y);
}

void mgdl_DrawRectangle(float x, float y, float w, float h, color32 color)
{
	glBegin(GL_QUADS);
		OpenGLRect(x,y,x+w,y-h,color);
	glEnd();
}
void mgdl_DrawRectangleV(Vector2 topleft, Vector2 size, color32 color)
{
	glBegin(GL_QUADS);
		OpenGLRect(topleft.x, topleft.y, topleft.x+size.x, topleft.y-size.y, color);
	glEnd();
}

void mgdl_DrawRectangleLines(float x, float y, float x2, float y2, color32 color)
{
	glBegin(GL_LINE_LOOP);
		OpenGLRect(x,y,x2,y2,color);
	glEnd();
}

void mgdl_DrawRectangleLinesEx(RectF rect, float lineThickness, color32 color)
{
	glLineWidth(lineThickness);
	glBegin(GL_LINE_LOOP);
		OpenGLRect(rect.x, rect.y, rect.x+rect.w, rect.y-rect.h, color);
	glEnd();
	glLineWidth(1.0f);
}

void mgdl_DrawLine ( float x, float y, float x2, float y2, color32 color )
{
	glBegin(GL_LINES);
		mgdl_glColor32(color);
		glVertex2f(x, y);
		glVertex2f(x2, y2);
	glEnd();
}

void mgdl_DrawLineV(Vector2 start, Vector2 end, color32 color )
{
	glBegin(GL_LINES);
		mgdl_glColor32(color);
		glVertex2f(start.x, start.y);
		glVertex2f(end.x, end.y);
	glEnd();
}

void mgdl_DrawText(float x, float y, const char* text, float fontSize, color32 color)
{
	Texture* db = DefaultFont_GetDefaultFont();
	Texture_DrawText(db, color, x, y, fontSize, text);
}

void mgdl_DrawTextV(Vector2 topleft, const char* text, float fontSize, color32 color)
{
	mgdl_DrawText((topleft.x), (topleft.y), text, fontSize, color);
}
