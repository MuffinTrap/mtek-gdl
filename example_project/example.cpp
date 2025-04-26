
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
    debugFont = GetDebugFont();

    blip = gdl::LoadSound("assets/blipSelect.wav");
    sampleMusic = gdl::LoadOgg("assets/sample3.ogg");

    wiiTexture = gdl::LoadImage("assets/wii_console_texture.png", gdl::TextureFilterModes::Nearest);
    matcapTexture = gdl::LoadImage("assets/matcap.png", gdl::TextureFilterModes::Linear);

    wiiScene = gdl::FBXFile::LoadFile("assets/wii_et_baby.fbx");
    wiiScene->SetMaterialTexture("wii_console_texture.png", wiiTexture);

    matcapMaterial = new gdl::Material("matcap", matcapTexture, gdl::MaterialType::Matcap);

    icosaScene = gdl::FBXFile::LoadFile("assets/ship_with_uvs.fbx");
    icosaScene->SetAllMaterialTextures(matcapTexture);
    gdl::Material* st = icosaScene->GetMaterial("standardSurface1");
    gdl::Material* mt2 = icosaScene->GetMaterial("Material.002");
    if (st!=nullptr)
    {
        st->type = gdl::MaterialType::Matcap;
    }
    if (mt2!=nullptr)
    {
        mt2->type = gdl::MaterialType::Matcap;
    }
    /*
    icosaScene = new gdl::Scene();
    icosaScene->AddMaterial(matcapMaterial);
  //  gdl::Mesh* quad = gdl::Mesh::CreateQuad(true, true);
   gdl::Mesh* icosaMesh = gdl::Mesh::CreateIcosahedron(true, true);
    gdl::Node* icosaNode = new gdl::Node("icosaNode", icosaMesh, matcapMaterial);
    icosaScene->AddChildNode(nullptr, icosaNode);
    */


    menu = gdl::MenuCreator(ibmFont, 1.0f, 1.0f);
    cameraMenu = gdl::MenuCreator(debugFont, 1.0f, 1.0f);

    musicLooping = sampleMusic->GetLooping();
    sceneRotation = vec3New(0.0f, 0.0f,0.0f);
    //quad->DebugPrint();
}

void Example::Update()
{

    // These are available only during update
    elapsedSeconds = gdl::GetElapsedSeconds();
    deltaTime = gdl::GetDeltaTime();

    static std::string babyName = "cuboid";
    gdl::Node* baby = wiiScene->GetNode(babyName);
    if (baby != nullptr)
    {
        baby->transform.rotationDegrees.x += deltaTime * 25.0f;
        baby->transform.rotationDegrees.z += deltaTime * 40.0f;
    }

    mouseClick = WiiController_ButtonPress(gdl::GetController(0), WiiButtons::ButtonA);
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

    vec3 s = vec3New(1, 1, 1);
    DrawScene(icosaScene, s);

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

    DrawCameraControls(gdl::GetScreenWidth()-80, top-120, 80);
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


void Example::DrawScene ( gdl::Scene* scene, const vec3& scale)
{
    // Try to draw Wii 3D model
    gdl::InitPerspectiveProjection(75.0f, 0.1f, 100.0f);
    gdl::InitCamera(vec3New(0.0f, 0.0f, cameraDistance), vec3New(0.0f, 0.0f, 0.0f), vec3New(0.0f, 1.0f, 0.0f));

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

    glTranslatef(0.0f, 0.0f, 0.0f);
    float elp = gdl::GetElapsedSeconds();
    glRotatef(elp * sceneRotation.x * 10.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(elp * sceneRotation.y * 10.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(elp * sceneRotation.z * 10.0f, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, scale.z);

    scene->Draw();

    glPopMatrix();
    glDisable(GL_DEPTH_TEST);

}

static void DrawButtons(short x, short y, short size, gdl::Font* font)
{
    // Draw button states
    gdl::Colors active = gdl::Colors::Blue;
    gdl::Colors inactive = gdl::Colors::LightBlue;
    int buttons[] = {
        WiiButtons::ButtonA,
        WiiButtons::ButtonB,
        WiiButtons::ButtonPlus,
        WiiButtons::ButtonMinus,
        WiiButtons::Button1,
        WiiButtons::Button2
        };
    static std::string names[] ={ "A", "B", "+", "-", "1", "2" };

    for(int i = 0; i < 6;i++ )
    {
        gdl::Colors c = inactive;
        if (WiiController_ButtonHeld(gdl::GetController(0), buttons[i]))
        {
            c = active;
        }
        DrawBoxF(x + i * size,
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
        WiiButtons::ButtonUp,
        WiiButtons::ButtonDown,
        WiiButtons::ButtonLeft,
        WiiButtons::ButtonRight
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
        if (WiiController_ButtonHeld(gdl::GetController(0), dpad_buttons[i]))
        {
            c = gdl::Colors::Red;
        }
        vec2 d=directions[i];
        DrawBoxF(x+d.x*box-h, y+ d.y*box-h, x+box+d.x*box-h, y+box+d.y*box-h, c);
    }
}

void DrawJoystick(short x, short y, short size)
{
    // Draw joystick direction
    short jsize=size;
    short box = jsize/3;
    short h=box/2;
    gdl::rgba8 jc = gdl::Colors::Green;
    vec2 jdir = WiiController_GetNunchukJoystickDirection(gdl::GetController(0));
    short jleft= x + jsize/2 + jdir.x * box-h;
    short jtop = y + jsize/2 + jdir.y * box-h;
    DrawBox(x, y, x+jsize, y+jsize, jc);
    if (jdir.x != 0.0f && jdir.y != 0.0f)
    {
        jc = gdl::Colors::LightGreen;
    }
    DrawBoxF(jleft, jtop, jleft+box, jtop+box,jc);
}

void Example::DrawInputInfo(int x, int y)
{
    // Draw cursor
    vec2 cp = WiiController_GetCursorPosition(gdl::GetController(0));

    pointerImage->Draw2DAligned(cp.x, cp.y, gdl::Colors::White, gdl::LJustify, gdl::LJustify);

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

    DrawBoxF(x, y, x+200, y - ystep* 4, gdl::Colors::Black);

    ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 0, scale,  "Deltatime %.4f", deltaTime);
    ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 1, scale, "Elapsed seconds: %.2f", elapsedSeconds);

    if (sampleMusic != nullptr)
    {
        ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 2, scale, "Music elapsed: %.2f", sampleMusic->GetElapsedSeconds());
        gdl::SoundStatus musicStatus = sampleMusic->GetStatus();
        gdl::rgba8 musicColor = gdl::Colors::Red;
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
        DrawBoxF(x-20, y-ystep*2, x, y-ystep*3, musicColor);
        debugFont->Icon(gdl::Colors::White, x-20, y-ystep*2, ystep, gdl::LJustify, gdl::LJustify, icon);
    }

    float blipElapsed = blip->GetElapsedSeconds();
    ibmFont->Printf(gdl::Colors::LightRed, x, y - ystep * 3, scale, "Sound elapsed: %.2f", blipElapsed);
    gdl::SoundStatus musicStatus = blip->GetStatus();
    gdl::rgba8 musicColor = gdl::Colors::Red;
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
    DrawBoxF(x-20, y-ystep*3, x, y-ystep*4, musicColor);
    debugFont->Icon(gdl::Colors::White, x-20, y-ystep*3, ystep, gdl::LJustify, gdl::LJustify, icon);
}

void Example::DrawMenu(int x, int y, int w)
{
    vec2 cp = WiiController_GetCursorPosition(gdl::GetController(0));

    // flip
    int h = gdl::GetScreenHeight();
    int flip_y = h-cp.y;

    menu.StartMenu(x, y, w, cp.x, flip_y, mouseClick);

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

void Example::DrawCameraControls(int x, int y, int w)
{
    vec2 cp = WiiController_GetCursorPosition(gdl::GetController(0));
    int h = gdl::GetScreenHeight();
    int flip_y = h-cp.y;

    cameraMenu.StartMenu(x, y, w, cp.x, flip_y, mouseClick);

    cameraMenu.Text("Control camera");
    if (cameraMenu.Button("Closer!"))
    {
        cameraDistance -= 1.0f;
    }
    if (cameraMenu.Button("Away!"))
    {
        cameraDistance += 1.0f;
    }
    if (cameraMenu.Button("Rotate Left"))
    {
        sceneRotation.y -= 1.0f;
    }
    if (cameraMenu.Button("Rotate right!"))
    {
        sceneRotation.y += 1.0f;
    }
    if (cameraMenu.Button("Rotate Up"))
    {
        sceneRotation.x -= 1.0f;
    }
    if (cameraMenu.Button("Rotate Down!"))
    {
        sceneRotation.x += 1.0f;
    }
    if (cameraMenu.Button("Rotate CW"))
    {
        sceneRotation.z -= 1.0f;
    }
    if (cameraMenu.Button("Rotate CCW!"))
    {
        sceneRotation.z += 1.0f;
    }
    if (cameraMenu.Button("Reset "))
    {
        sceneRotation = vec3Zero();
    }
}

