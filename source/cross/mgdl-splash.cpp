#include <mgdl/mgdl-splash.h>
#include <mgdl/ccVector/ccVector.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-debugfont.h>
#include <mgdl/mgdl-config.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-draw2d.h>

static float areaHeight;
static float lineHeight;
static float lineWidth;
static float areaTop;
static float areaBottom;
static float animationProgress = 0.0f;

	// lean is in pixels
static void GetCorners(float xOffset, float start, float height, float* cornersOut, float lean)
{
	// The lean needs to be the same regardless of segment height
	vec2 AreaTopLeft = vec2New(xOffset + lean, areaTop);
	vec2 AreaBottomLeft = vec2New(xOffset, areaBottom);

	// down = bottomleft - topLeft
	vec2 downwards = vec2Add(AreaBottomLeft, vec2Negate(AreaTopLeft));
	// Lean vector points left and down
	vec2 D = vec2Normalize(downwards);


	// From area top left go down until segment starts
	vec2 tl = vec2Add(AreaTopLeft, vec2Multiply(D, (float)(start * lineHeight)));
	cornersOut[0] = tl.x;
	cornersOut[1] = tl.y;

	// From segment start go down * height
	vec2 bl = vec2Add(tl, vec2Multiply(D, (float)(height * lineHeight)));
	cornersOut[2] = bl.x;
	cornersOut[3] = bl.y;

	// Bottom right
	cornersOut[4] = bl.x + lineWidth;
	cornersOut[5] = bl.y;

	// Top right
	cornersOut[6] = tl.x + lineWidth;
	cornersOut[7] = tl.y;
}

static float DrawLetter(float x, u8* sl, u8 bars, u32 color, float lean)
{
	float startX = x;
	float corners[] = {0,0,0,0, 0,0,0,0};
	for (int i = 0; i < bars; i++)
	{
		int line = i *2;
		GetCorners(x, sl[line+0], sl[line+1], corners, lean);
		gdl::DrawQuad(corners[0], corners[1], corners[2], corners[3],
					  corners[4], corners[5], corners[6], corners[7],
					  color);
		x += lineWidth-1;
	}
	return x - startX;
}
static void DrawLetters(float x, float lean, u32 color, gdl::Font* font, float baseLine)
{
	// m
	// start points and lengths
	// start points 0, 1, 2,
	// lenghts : o height = 8
	u8 m_ls[] = {
		8, 1,
		8, 8,
		8, 1,
		9, 7,
		8, 1,
		9, 7,
		15, 1}; // leads to g

	// g in three parts
	u8 g_outer[] = {8,8,
		8, 1,
		8, 1,
		8, 16,
	};
	u8 g_under[] = {0,0,
		15, 1,
		15, 1,
		0,0};
	u8 gl_foot[] = {8+8+4, 4,
		23, 1,
		23, 1,
		0,0,
		8, 1  // Leads to d
	};

	// d in two parts
	u8 d_outer[] = {
		8,8,
		8,1,
		8,1,
		0, 16};

	u8 d_under[] = {
		0, 0,
		15, 1,
		15, 1,
		0,0,
		15, 1}; // leads to l

	u8 l_ls[] = {
		0, 16,
		15,1,
	};

	float drawX = x;
		drawX += DrawLetter(drawX, m_ls, 7, color, lean);
		DrawLetter(drawX, g_outer, 4, color, lean);
		DrawLetter(drawX, g_under, 4, color, lean);
		drawX += DrawLetter(drawX, gl_foot, 5, color, lean) -1;
		float dStart = drawX + lineWidth;
		DrawLetter(drawX, d_outer, 4, color, lean);
		drawX += DrawLetter(drawX, d_under, 5, color, lean) -1;
		drawX += DrawLetter(drawX, l_ls, 2, color, lean);
	font->Print(color, dStart, baseLine, 8, GDL_VERSION);
	font->Print(color, dStart, baseLine - 8, 8, gdl::Platform::GetPlatform().GetName().c_str());
}

void gdl::DrawSplashScreen(float deltaTime)
{
	// Draws mgdl
	// in stylized letters
	gdl::InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	gdl::Palette* blessing = gdl::Palette::GetDefaultPalette();
	gdl::Font* debf = gdl::GetDebugFont();
	u32 yellow = blessing->GetColor(3);
	u32 grey = blessing->GetColor(6);


	u16 sw = gdl::GetScreenWidth();
	u16 sh = gdl::GetScreenHeight();

	// Logo takes up 6/4 of screen height
	areaHeight = (sh/6.0f) * 4.0f;
	// 3/4 screen width
	float areaWidth = (sw/4.0f) * 3.0f;

	// Space above and below logo
	float paddingY = (sh - areaHeight)/2.0f;
	areaBottom = paddingY;
	areaTop = areaBottom + areaHeight;

	// Logo is made of 19 segments
	float segments = 19.0f;
	lineWidth = (areaWidth) / segments;
	float oHeight = areaHeight / 3.0f;
	// Letter o is 8 lines tall
	// extensions are 8 lines in both directions
	lineHeight = oHeight / 8.0f;

	// Where the characters are drawn: ________________
	float baseLine = areaBottom + oHeight;

	// How much more the position on topline is to the right
	float lean = lineWidth * 3.0f;

	// Estimate of logowidth
	float logoWidth = lineWidth * segments + lean;

	// Space on both sides of logo
	float paddingX = (gdl::GetScreenWidth() - logoWidth) / 2;

	DrawLetters(paddingX, lean, grey, debf, baseLine);

	float leftX = 0;
	float rightX = 0;
	animationProgress += deltaTime * 0.5f;
	if (animationProgress <= 1.0f)
	{
		leftX = 0.0f;
		rightX = sw * animationProgress;
	}
	else if (animationProgress <= 2.0f)
	{
		rightX = sw;
		leftX = (animationProgress-1.0f) * sw;
	}
	else
	{
		animationProgress = 0.0f;
		leftX = 0.0f;
	}
	glEnable(GL_SCISSOR_TEST);
	glScissor(leftX, 0, rightX, sh);
	DrawLetters(paddingX, lean, yellow, debf, baseLine);
	glScissor(0, 0, sw, sh);
	glDisable(GL_SCISSOR_TEST);
}
