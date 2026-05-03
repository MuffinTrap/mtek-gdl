#include "ccVector.angel"

#if USE_ANGEL_AS_CPP
#	include <mgdl.h>
#	include <mgdl/mgdl-script-api.h>
#	include "angel.hxx"
#	include <mgdl/mgdl-angelscript.h>
#	ifdef __cplusplus
		extern "C" {
#	endif
#endif

float elapsed = 0.0f;
vec2 deltaCircle;
vec2 frameCircle;
const float speed = 10.0f;
const float circleSize = 10.0f;

TextureHandle barb;

void angelscript_init()
{
	int screenWidth = mgdl_GetScreenWidth();
	int screenHeight = mgdl_GetScreenHeight();
	deltaCircle = vec2New(0, screenHeight/4.0f );
	frameCircle = vec2New(0, screenHeight*(2.0f/3.0f) );

	barb = mgdl_LoadTexture("assets/barb.png");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	PaletteHandle debugPalette = mgdl_GetDebugPalette();
	mgdl_SetPalette(debugPalette);
}

void angelscript_quit()
{

}

void effect_2d(float deltatime)
{
	glClearColor(0.2f, 0.2f, 0.1f, 1.0f);
	mgdl_InitOrthoProjection();
	int screenWidth = mgdl_GetScreenWidth();
	int screenHeight = mgdl_GetScreenHeight();
	deltaCircle.x += deltatime * speed;
	frameCircle.x += 0.01f * speed;

	if (deltaCircle.x > screenWidth || mgdl_IsButtonPressed(0, ButtonA))
	{
		deltaCircle.x = 0;
	}
	if (frameCircle.x > screenWidth || mgdl_IsButtonPressed(0, ButtonA))
	{
		frameCircle.x = 0;
	}


	mgdl_DrawTexture(barb, 120, screenHeight-16);

	mgdl_DrawRectangle(deltaCircle.x, deltaCircle.y, circleSize, circleSize, 3 );
	mgdl_DrawRectangle(frameCircle.x, frameCircle.y, circleSize, circleSize, 4 );
}
void Quad(
    vec3 A,
    vec3 B,
    vec3 C,
    vec3 D)
{
	// bottom
	glVertex3f(A.x, A.y, A.z);

	// bottom2
	glVertex3f(D.x, D.y, D.z);

	// top 1 and 2
	glVertex3f(C.x, C.y, C.z);

	glVertex3f(B.x, B.y, B.z);
}
void effect_3d(float deltatime)
{
	elapsed += deltatime;
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	int screenWidth = mgdl_GetScreenWidth();
	int screenHeight = mgdl_GetScreenHeight();
	float aspect = float(screenWidth)/float(screenHeight);
	float nearZ = 0.01f;
	float farZ = 10.0f;
    gluPerspective(60.0f, aspect, nearZ, farZ);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 4.0f,
				 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0);

	glRotatef(elapsed * 90.0f, 1.0f, 0.0f, 0.3f);

	float size = 1.0f;
	float hs = size/2.0f;
	vec3 bl = vec3New(-hs, -hs, -hs);
	vec3 br = vec3New( hs, -hs, -hs);
	vec3 fl = vec3New(-hs, -hs,  hs);
	vec3 fr = vec3New( hs, -hs,  hs);

	vec3 tbl = vec3New(-hs, hs, -hs); // top back left
	vec3 tbr = vec3New( hs, hs, -hs); // top back right
	vec3 tfl = vec3New(-hs, hs, hs); //  top forward left
	vec3 tfr = vec3New( hs, hs, hs);

	glBegin(GL_QUADS);
		// left side
		glColor3f(0.5f, 0.5f, 0.1f);
		Quad(bl, tbl, tfl, fl);
		// right
		glColor3f(0.2f, 0.8f, 0.4f);
		Quad(fr, tfr, tbr, br);

		// front
		glColor3f(0.9f, 0.2f, 0.1f);
		Quad(fl, tfl, tfr, fr);
		// back
		glColor3f(0.5f, 0.4f, 0.2f);
		 Quad(br, tbr, tbl, bl);

         // top
		glColor3f(0.7f, 0.1f, 0.5f);
        Quad(tfl, tbl, tbr, tfr);
		 // bottom
		glColor3f(0.8f, 0.8f, 0.4f);
		Quad(fr, br, bl, fl);
	glEnd();
}

void angelscript_frame(float deltatime)
{
	effect_2d(deltatime);
}

#if USE_ANGEL_AS_CPP
#	ifdef __cplusplus
		}
#	endif
#endif
