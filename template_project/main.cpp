#include <mgdl.h>

// TODO Move this over to platform too?
static gdl::WiiController controller;

void init()
{
    glViewport(0, 0, gdl::GetScreenWidth(), gdl::GetScreenHeight());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 2D mode
    /*
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double)plat.GetScreenWidth(), 0.0, (double)plat.GetScreenHeight());
    */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, gdl::GetAspectRatio(), 0.1, 100.0);

    glClearColor(247.0f/255.0f, 1.0f, 174.0f/255.0f, 0.0f);
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        // 2D cross
        /*
        gdl::Platform& plat = gdl::Platform::GetPlatform();
        glVertex2f(0.0f, 0.0f);
        glVertex2f((float)plat.GetScreenWidth(), (float)plat.GetScreenHeight());

        glVertex2f(0.0f, (float)plat.GetScreenHeight());
        glVertex2f((float)plat.GetScreenWidth(), 0.0f);
        */

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();
}

// Called before render()
void update()
{
    // TODO Make the Platform functions available in gdl namespace directly
    gdl::ReadController(controller);
    if (controller.ButtonPress(gdl::WiiButtons::ButtonHome))
    {
        gdl::Platform& plat = gdl::Platform::GetPlatform();
        plat.DoProgramExit();
    }
}

int main()
{
    gdl::InitSystem(
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render,
        gdl::PlatformInitFlag::FlagPauseUntilA
    );
}
