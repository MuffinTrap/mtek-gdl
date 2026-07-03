#include "mgdl.angel"
#include "raymath.angel"

#if USE_ANGEL_AS_CPP
#	include <mgdl.h>
#	include <mgdl/raymath/raymath.h>
#	include <mgdl/mgdl-script-api.h>
#	include "angel.hpp"
#	include <mgdl/mgdl-angelscript.h>
#	ifdef __cplusplus
		extern "C" {
#	endif
#endif

float elapsed = 0.0f;
Vector2 deltaCircle;
Vector2 frameCircle;
const float speed = 10.0f;
const float circleSize = 10.0f;

TextureHandle barb;
PaletteHandle debugPalette;
ModelHandle wiiModel;
SceneHandle lightScene;

void angelscript_init()
{
	int screenWidth = mgdl_GetScreenWidth();
	int screenHeight = mgdl_GetScreenHeight();

	wiiModel = mgdl_LoadModel("assets/wii_console.fbx");

	deltaCircle = Vector2New(0, screenHeight/4.0f );
	frameCircle = Vector2New(0, screenHeight*(2.0f/3.0f) );

	barb = mgdl_LoadTexture("assets/barb.png");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	debugPalette = mgdl_GetDebugPalette();
}

void angelscript_quit()
{

}

void setup_3d()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE); //  is this needed?

	// This is the other way around on Wii, but
	// hopefully OpenGX handles it
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	mgdl_SetGlobalAmbientColor32(Debug_White, 0.2f);

    glColor3f(1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	int screenWidth = mgdl_GetScreenWidth();
	int screenHeight = mgdl_GetScreenHeight();
	float aspect = float(screenWidth)/float(screenHeight);
	float nearZ = 0.01f;
	float farZ = 100.0f;
    gluPerspective(60.0f, aspect, nearZ, farZ);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(0.0f, 5.0f, 10.0f,
				 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0);

}

void scene_test_scene(float deltatime)
{

	elapsed += deltatime;
	setup_3d();

	glPushMatrix();
	glRotatef(elapsed * 90.0f, 0.0f, 1.0f, 0.0f);

	mgdl_DrawScene(lightScene, 0,0,0, 1.0f, Debug_White);
	glPopMatrix();

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

	mgdl_DrawRectangle(deltaCircle.x, deltaCircle.y, circleSize, circleSize, mgdl_GetPaletteColor(debugPalette,3) );
	mgdl_DrawRectangle(frameCircle.x, frameCircle.y, circleSize, circleSize, mgdl_GetPaletteColor(debugPalette,4) );

	mgdl_DrawText("Henlo!", 100, 300 + sin(mgdl_GetElapsedSeconds() * 2) * 130, 64, Debug_Yellow);
}
void Quad(
    Vector3 A,
    Vector3 B,
    Vector3 C,
    Vector3 D)
{
	// bottom
	glVertex3f(A.x, A.y, A.z);

	// bottom2
	glVertex3f(D.x, D.y, D.z);

	// top 1 and 2
	glVertex3f(C.x, C.y, C.z);

	glVertex3f(B.x, B.y, B.z);
}

void effect_wii_scene(float deltatime)
{
	elapsed += deltatime;
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	int screenWidth = mgdl_GetScreenWidth();
	int screenHeight = mgdl_GetScreenHeight();
	float aspect = float(screenWidth)/float(screenHeight);
	float nearZ = 0.01f;
	float farZ = 200.0f;
    gluPerspective(60.0f, aspect, nearZ, farZ);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 45.0f,
				 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0);

	glPushMatrix();

	glRotatef(elapsed * 90.0f, 1.0f, 0.0f, 0.3f);
	glScalef(0.01f, 0.01f, 0.01f);
	mgdl_DrawModel(wiiModel, 0,0,0, 1.0f, Debug_White);
	glPopMatrix();

}

void effect_3d(float deltatime)
{
	elapsed += deltatime;
	setup_3d();

	// Lighting done with OpenGL
	glEnable(GL_LIGHTING);
	GLfloat[] white = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat[] zero = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_POSITION, zero);
	glEnable(GL_LIGHT0);

	GLfloat[] green = {0.0f, 1.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_AMBIENT, green);


	glPushMatrix();
	glRotatef(elapsed * 90.0f, 1.0f, 0.0f, 0.0f);


	glPushMatrix();
	glTranslatef(1.0f, 0.0f, 0.0f);
	glRotatef(elapsed * 90.0f, 1.0f, 0.0f, 0.3f);

	float size = 1.0f;
	float hs = size/2.0f;
	Vector3 bl = Vector3New(-hs, -hs, -hs);
	Vector3 br = Vector3New( hs, -hs, -hs);
	Vector3 fl = Vector3New(-hs, -hs,  hs);
	Vector3 fr = Vector3New( hs, -hs,  hs);

	Vector3 tbl = Vector3New(-hs, hs, -hs); // top back left
	Vector3 tbr = Vector3New( hs, hs, -hs); // top back right
	Vector3 tfl = Vector3New(-hs, hs, hs); //  top forward left
	Vector3 tfr = Vector3New( hs, hs, hs);

	// NOTE
	// Normals are needed for lighting to work
	glBegin(GL_QUADS);
		// left side
		glColor3f(0.5f, 0.5f, 0.1f);
		glNormal3f(-1.0f, 0.0f, 0.0);
		Quad(bl, tbl, tfl, fl);
		// right
		glNormal3f(1.0f, 0.0f, 0.0);
		glColor3f(0.2f, 0.8f, 0.4f);
		Quad(fr, tfr, tbr, br);

		// front
		glNormal3f(0.0f, 0.0f, -1.0);
		glColor3f(0.9f, 0.2f, 0.1f);
		Quad(fl, tfl, tfr, fr);
		// back
		glNormal3f(0.0f, 0.0f, 1.0);
		glColor3f(0.5f, 0.4f, 0.2f);
		 Quad(br, tbr, tbl, bl);

         // top
		glNormal3f(0.0f, 1.0f, 0.0);
		glColor3f(0.7f, 0.1f, 0.5f);
        Quad(tfl, tbl, tbr, tfr);
		 // bottom
		glNormal3f(0.0f, -1.0f, 0.0);
		glColor3f(0.8f, 0.8f, 0.4f);
		Quad(fr, br, bl, fl);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_LIGHTING);
}

void angelscript_frame(float deltatime)
{
	//effect_3d(deltatime);
	effect_2d(deltatime);
	//scene_test_scene(deltatime);
	//effect_scene(deltatime);
}

#if USE_ANGEL_AS_CPP
#	ifdef __cplusplus
		}
#	endif
#endif
