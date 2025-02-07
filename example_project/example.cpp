
#include "example.h"
#include <mgdl/mgdl-draw2d.h>

#include <string>

Example::Example()
{

}

void Example::Init()
{
    short spriteHeight = 64;
    barb = gdl::LoadImage("assets/barb.png", gdl::TextureFilterModes::Linear);
    mel_sprites.LoadFromImage("assets/mel_tiles.png", spriteHeight, spriteHeight);
    pointerImage = gdl::LoadImage("assets/pointer.png", gdl::TextureFilterModes::Nearest);
    ibmFont = gdl::LoadFont("assets/font8x16.png", 8, 16, ' ');
    debugFont = gdl::GetDebugFont();

    blip = gdl::LoadSound("assets/blipSelect.wav");
    sampleMusic = gdl::LoadOgg("assets/sample3.ogg");

    wiiTexture = gdl::LoadImage("assets/wii_console_texture.png", gdl::TextureFilterModes::Nearest);

    gdl::FBXFile* wiiFbx = new gdl::FBXFile();
    wiiScene = wiiFbx->LoadFile("assets/wii_et_baby.fbx");
    wiiScene->SetMaterialTexture("wii_console_texture.png", wiiTexture);


    menu = gdl::MenuCreator(ibmFont, 1.0f, 1.0f);

    musicLooping = sampleMusic->GetLooping();
}

void Example::Update()
{

    // These are available only during update
    elapsedSeconds = gdl::GetElapsedSeconds();
    deltaTime = gdl::GetDeltaTime();

    static std::string babyName = "cuboid";
    gdl::Node* baby = wiiScene->GetNode(babyName);
    baby->transform.rotationDegrees.x += deltaTime * 25.0f;
    baby->transform.rotationDegrees.z += deltaTime * 40.0f;

    mouseClick = gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonA);
}

void DrawTextDouble(const char* text, short x, short y, float textHeight, gdl::Font* font)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        font->Print(gdl::Colors::Black, textHeight, gdl::LJustify, gdl::LJustify, text);

        glTranslatef(-1, -1, 0.0f);
        font->Print(gdl::Colors::LightGreen, textHeight, gdl::LJustify, gdl::LJustify, text);
    glPopMatrix();
}

void Example::Draw()
{
    gdl::InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DrawImage();

    DrawWii();

    gdl::InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Input
    short sh = gdl::GetScreenHeight();
    short top = sh - 32;
    short left = 22;
    DrawMenu(left, top - 120, 120);
    DrawSprites();
    DrawTimingInfo(left,
                   gdl::GetScreenHeight()/5,
                   ibmFont->GetCharacterHeight());
    DrawVersion();
    DrawInputInfo(left, top);
}

void Example::DrawVersion()
{
    float textHeight = debugFont->GetCharacterHeight() * 2;
    short sh = gdl::GetScreenHeight();
    int CenterX = gdl::GetScreenWidth()/4;
    DrawTextDouble("MTEK-GDL", CenterX, sh - textHeight, textHeight, debugFont);
    DrawTextDouble(GDL_VERSION, CenterX, sh - textHeight * 2, textHeight, debugFont);
}


void Example::DrawImage()
{
    // Draw Image
    barb->Draw2DAligned(
            0,
            gdl::GetScreenHeight()/2,
            1.0f,
            gdl::LJustify, gdl::Centered);
}


void Example::DrawWii()
{
    // Try to draw Wii 3D model
    gdl::InitPerspectiveProjection(75.0f, 0.1f, 100.0f);
    gdl::InitCamera(vec3New(0.0f, 0.0f, 0.0f), vec3New(0.0f, 0.0f, -1.0f), vec3New(0.0f, 1.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE); //  is this needed?

	// This is the other way around on Wii, but
	// hopefully OpenGX handles it
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_FLAT);

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();

    glTranslatef(5.0f, -5.0f, -12.0f);
    glRotatef(elapsedSeconds * 10.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.1f, 0.1f, 0.1f);

    wiiScene->Draw();

    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
}

static void DrawButtons(short x, short y, short size, gdl::Font* font)
{
    // Draw button states
    gdl::Colors active = gdl::Colors::Blue;
    gdl::Colors inactive = gdl::Colors::LightBlue;
    int buttons[] = {
        gdl::WiiButtons::ButtonA,
        gdl::WiiButtons::ButtonB,
        gdl::WiiButtons::ButtonPlus,
        gdl::WiiButtons::ButtonMinus,
        gdl::WiiButtons::Button1,
        gdl::WiiButtons::Button2
        };
    static std::string names[] ={ "A", "B", "+", "-", "1", "2" };

    for(int i = 0; i < 6;i++ )
    {
        gdl::Colors c = inactive;
        if (gdl::GetController(0).ButtonHeld(buttons[i]))
        {
            c = active;
        }
        gdl::DrawBoxF(x + i * size,
                      y,
                      x+size+i*size,
                      y-size,c);
        font->Print(gdl::Colors::White, x + i * size, y, 16.0f, gdl::LJustify, gdl::LJustify, names[i].c_str());
    }
}

void DrawDPad(short x, short y, short size)
{
    short box = size/3;
    short h=box/2;
    x += box + h;
    y += box + h;
    // Dpad
    int dpad_buttons[] = {
        gdl::WiiButtons::ButtonUp,
        gdl::WiiButtons::ButtonDown,
        gdl::WiiButtons::ButtonLeft,
        gdl::WiiButtons::ButtonRight
    };
    vec2 directions[] = {
        vec2New(0,-1),
        vec2New(0,1),
        vec2New(-1,0),
        vec2New(1,0)
    };
    for (int i=0;i<4;i++)
    {
        gdl::Colors c = gdl::Colors::LightRed;
        if (gdl::GetController(0).ButtonHeld(dpad_buttons[i]))
        {
            c = gdl::Colors::Red;
        }
        vec2 d=directions[i];
        gdl::DrawBoxF(x+d.x*box-h, y+ d.y*box-h, x+box+d.x*box-h, y+box+d.y*box-h, c);
    }
}

void DrawJoystick(short x, short y, short size)
{
    // Draw joystick direction
    short jsize=size;
    short box = jsize/3;
    short h=box/2;
    gdl::Color jc = gdl::Colors::Green;
    gdl::ControllerVec2 jdir = gdl::GetController(0).GetNunchukJoystickDirection(0.0f);
    short jleft= x + jsize/2 + jdir.xAxis * box-h;
    short jtop = y + jsize/2 + jdir.yAxis * box-h;
    gdl::DrawBox(x, y, x+jsize, y+jsize, jc);
    if (jdir.xAxis != 0.0f && jdir.yAxis != 0.0f)
    {
        jc = gdl::Colors::LightGreen;
    }
    gdl::DrawBoxF(jleft, jtop, jleft+box, jtop+box,jc);
}

void Example::DrawInputInfo(int x, int y)
{
    // Draw cursor
    gdl::ControllerVec2 cp = gdl::GetController(0).GetCursorPosition();

    pointerImage->Draw2DAligned(cp.xAxis, cp.yAxis, gdl::Colors::White, gdl::LJustify, gdl::LJustify);

    DrawButtons(x, y, 20, ibmFont);
    y += 20;
    DrawDPad(x, y, 60);
    x += 80;
    DrawJoystick(x, y, 60);
}

void Example::DrawSprites()
{
    const short h = mel_sprites.GetSpriteHeight();
    const short w = mel_sprites.GetSpriteWidth();
    float scale = 2.0f;
    short spriteW = w * scale;
    short spriteH = h * scale;
    short placeX = gdl::GetScreenWidth() - spriteW;
    short placeY = gdl::GetScreenHeight();
    for (short i = 0; i < 4; i++)
    {
        mel_sprites.Draw2D(i, placeX, placeY, spriteH, gdl::LJustify, gdl::LJustify, gdl::Colors::White);
        placeY -= spriteH;
    }
}

void Example::DrawTimingInfo(int x, int y, float scale)
{
    float ystep = ibmFont->GetCharacterHeight();

    gdl::DrawBoxF(x, y, x+200, y - ystep* 4, gdl::Colors::Black);

    ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 0, scale,  "Deltatime %.4f", deltaTime);
    ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 1, scale, "Elapsed seconds: %.2f", elapsedSeconds);

    if (sampleMusic != nullptr)
    {
        ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 2, scale, "Music elapsed: %.2f", sampleMusic->GetElapsedSeconds());
        gdl::SoundStatus musicStatus = sampleMusic->GetStatus();
        gdl::Color musicColor = gdl::Colors::Red;
        gdl::IconSymbol icon = gdl::IconSymbol::Dot;

        switch(musicStatus)
        {
            case gdl::SoundStatus::Playing:
                musicColor = gdl::Colors::Green;
                icon = gdl::IconSymbol::TriangleRight;
                break;
            case gdl::SoundStatus::Paused:
                musicColor = gdl::Colors::Yellow;
                icon = gdl::IconSymbol::TriangleVertical;
                break;
            case gdl::SoundStatus::Stopped: musicColor = gdl::Colors::Red;
                icon = gdl::IconSymbol::BlockUnder;
                break;
            case gdl::SoundStatus::Initial: musicColor = gdl::Colors::Black; break;
        };
        gdl::DrawBoxF(x-20, y-ystep*2, x, y-ystep*3, musicColor);
        debugFont->Icon(gdl::Colors::White, x-20, y-ystep*2, ystep, gdl::LJustify, gdl::LJustify, icon);
    }

    float blipElapsed = blip->GetElapsedSeconds();
    ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 3, scale, "Sound elapsed: %.2f", blipElapsed);
    gdl::SoundStatus musicStatus = blip->GetStatus();
    gdl::Color musicColor = gdl::Colors::Red;
    gdl::IconSymbol icon = gdl::IconSymbol::Dot;
    switch(musicStatus)
    {
        case gdl::SoundStatus::Playing:
            musicColor = gdl::Colors::Green;
            icon = gdl::IconSymbol::TriangleRight;
            break;
        case gdl::SoundStatus::Paused:
            musicColor = gdl::Colors::Yellow;
            icon = gdl::IconSymbol::TriangleVertical;
            break;
        case gdl::SoundStatus::Stopped: musicColor = gdl::Colors::Red;
            icon = gdl::IconSymbol::BlockUnder;
            break;
        case gdl::SoundStatus::Initial: musicColor = gdl::Colors::Black; break;
    };
    gdl::DrawBoxF(x-20, y-ystep*3, x, y-ystep*4, musicColor);
    debugFont->Icon(gdl::Colors::White, x-20, y-ystep*3, ystep, gdl::LJustify, gdl::LJustify, icon);
}

void Example::DrawMenu(int x, int y, int w)
{
    gdl::ControllerVec2 cp = gdl::GetController(0).GetCursorPosition();

    // flip
    int h = gdl::GetScreenHeight();
    int flip_y = h-cp.yAxis;

    menu.StartMenu(x, y, w, cp.xAxis, flip_y, mouseClick);

    menu.Text("Hi! I am menu.");
    menu.Panel(2, gdl::Colors::Yellow);
    if (menu.Button("Play Ogg"))
    {
        sampleMusic->Play();
    }
    if (menu.Button("Pause Ogg"))
    {
        bool ispaused = sampleMusic->GetStatus() == gdl::SoundStatus::Paused;
        sampleMusic->SetPaused(!ispaused);
    }
    if (menu.Button("Stop Ogg"))
    {
        sampleMusic->Stop();
    }
    if (menu.Toggle("Loop Ogg", musicLooping ))
    {
        sampleMusic->SetLooping(musicLooping);
    }
    if (menu.Button("Play Sound"))
    {
        blip->Play(1.0f, 100.0f);
    }
    if (menu.Button("Fire Assert"))
    {
        gdl_assert_print(false, "Assert button pressed!");
    }
}
