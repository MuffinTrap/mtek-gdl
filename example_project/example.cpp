
#include "example.h"
#include <mgdl/mgdl-draw2d.h>

#include <string>

Example::Example()
{

}

void Example::Init()
{
    // Sprites, images and fonts
    short spriteHeight = 64;
    barb = LoadImage("assets/barb.png", gdl::TextureFilterModes::Linear);
    mel_sprites = LoadSprite("assets/mel_tiles.png", spriteHeight, spriteHeight);
    fruitSprites = LoadSprite("assets/fruits.png", 16, 16);
    pointerImage = LoadImage("assets/pointer.png", gdl::TextureFilterModes::Nearest);
    ibmFont = LoadFont("assets/font8x16.png", 8, 16, ' ');
    debugFont = Font_GetDebugFont();

    // Audio
    blip = LoadSound("assets/blipSelect.wav");
    sampleMusic = LoadOgg("assets/sample3.ogg");

    // Wii model scene
    wiiScene = LoadFBX("assets/wii_et_baby.fbx");
    wiiTexture = LoadImage("assets/wii_console_texture.png", gdl::TextureFilterModes::Nearest);
    Scene_SetMaterialTexture(wiiScene, "wii_console_texture.png", wiiTexture);

    // Ship with matcap texture
    shipScene = LoadFBX("assets/ship_with_uvs.fbx");
    matcapTexture = LoadImage("assets/matcap.png", gdl::TextureFilterModes::Linear);
    matcapMaterial = Material_Load("matcap", matcapTexture, gdl::MaterialType::Matcap);
    Scene_SetAllMaterialTextures(shipScene, matcapTexture);
    Material* st = Scene_GetMaterial(shipScene, "standardSurface1");
    Material* mt2 = Scene_GetMaterial(shipScene, "Material.002");
    if (st!=nullptr)
    {
        st->type = gdl::MaterialType::Matcap;
    }
    if (mt2!=nullptr)
    {
        mt2->type = gdl::MaterialType::Matcap;
    }

    // Generated icosahedron and checkerboard texture
    icosaScene = Scene_CreateEmpty();
    checkerTexture = Image_GenerateCheckerBoard();
    Material* checkerMaterial = Material_Load("checker", checkerTexture, gdl::MaterialType::Diffuse);

    Scene_AddMaterial(icosaScene, checkerMaterial );
    Mesh* quad = Mesh_CreateQuad(FlagNormals | FlagUVs);
    // Mesh* icosaMesh = Mesh_CreateIcosahedron(FlagNormals | FlagUVs);
    Node* icosaNode = Node_Create();
    Node_SetContent(icosaNode, "icosaNode", quad, checkerMaterial);
    Scene_AddChildNode(icosaScene, nullptr, icosaNode);


    menu = Menu_CreateWindowed(ibmFont, 1.0f, 1.0f, 128, 256, "MTEK GDL");
    cameraMenu = Menu_Create(debugFont, 1.0f, 1.0f);
    controllerMenu = Menu_Create(ibmFont, 1.0f, 1.0f);
    performanceMenu = Menu_Create(debugFont, 1.0f, 1.0f);
    audioMenu = Menu_Create(debugFont, 1.0f, 1.0f);

    musicLooping = Music_GetLooping(sampleMusic);
    sceneRotation = V3f_Create(0.0f, 1.0f,0.0f);
    //quad->DebugPrint();

    cameraDistance = 30.0f;
}

void Example::Update()
{

    // These are available only during update
    elapsedSeconds = mgdl_GetElapsedSeconds();
    deltaTime = mgdl_GetDeltaTime();

    Menu_ReadDefaultInputs();

    cursorPos = WiiController_GetCursorPosition(Platform_GetController(0));
    mouseClick = WiiController_ButtonPress(Platform_GetController(0), ButtonA);
    mouseDown = WiiController_ButtonHeld(Platform_GetController(0), ButtonA);
    /*
    static const char* babyName = "cuboid";
    Node* baby = Scene_GetNode(wiiScene, babyName);
    if (baby != nullptr)
    {
        baby->transform->rotationDegrees.x += deltaTime * 25.0f;
        baby->transform->rotationDegrees.z += deltaTime * 40.0f;
    }
    */
}
#if 0

void DrawTextDouble(const char* text, short x, short y, float textHeight, gdl::Font* font)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        font->Print(gdl::Colors::Black, textHeight, gdl::LJustify, gdl::LJustify, text);

        glTranslatef(-1, -1, 0.0f);
        font->Print(gdl::Colors::LightGreen, textHeight, gdl::LJustify, gdl::LJustify, text);
    glPopMatrix();
}
#endif

void Example::Draw()
{
    mgdl_InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    if ( toggleSprites) {DrawSprites();}
    if ( toggle3D) {DrawIcosa();}
    if ( toggleImage) {DrawImage();}
    if ( toggleCamera) {DrawCameraControls();}
    if ( toggleInputs) {DrawInputInfo();}
    if ( togglePerformance) {DrawTimingInfo();}
    if ( toggleAudio) {DrawAudio();}


    mgdl_InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DrawMenu();

    Image_Draw2DAligned(pointerImage, cursorPos.x, cursorPos.y, gdl::Colors::White, gdl::LJustify, gdl::LJustify);
    Font_Printf(debugFont, Colors::White, 10, 16, 16, "Mouse down: %d, click %d", mouseDown, mouseClick);
}

void Example::DrawSprites()
{
    mgdl_glSetAlphaTest(true);
    mgdl_glSetTransparency(true);
    for (int i = 0; i < 16; i++)
    {
        int size = 64;
        Sprite_Draw2D(fruitSprites, i, size * (i%4), size + (i/4) * size, size, gdl::LJustify, gdl::RJustify, Colors::White);
    }

    const short h = Sprite_GetHeight(mel_sprites);
    const short w = Sprite_GetHeight(mel_sprites);
    float scale = 2.0f;
    short spriteW = w * scale;
    short spriteH = h * scale;
    short placeX = mgdl_GetScreenWidth() - spriteW;
    short placeY = mgdl_GetScreenHeight();
    for (short i = 0; i < 4; i++)
    {
        Sprite_Draw2D(mel_sprites, i, placeX, placeY, spriteH, gdl::LJustify, gdl::LJustify, gdl::Colors::White);
        placeY -= spriteH;
    }
}

void Example::DrawIcosa()
{
    cameraDistance = 5.0f;
    DrawScene(icosaScene, V3f_Create(1.0f, 1.0f, 1.0f));
    cameraDistance = 15.0f;
    DrawScene(wiiScene, V3f_Create(0.1f, 0.1f, 0.1f));
}

void Example::DrawImage()
{
    // Draw Image
    Image_Draw2DAligned(barb,
            0,
            mgdl_GetScreenHeight()/2,
            1.0f,
            gdl::LJustify, gdl::Centered);

    Image_Draw2DAligned(debugFont->_fontImage,
            0,
            mgdl_GetScreenHeight()/2,
            1.0f,
            gdl::LJustify, gdl::Centered);
}

#if 0

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

#endif


void Example::DrawScene ( Scene* scene, vec3 scale)
{
    // Try to draw Wii 3D model
    mgdl_InitPerspectiveProjection(75.0f, 0.1f, 100.0f);
    mgdl_InitCamera(V3f_Create(0.0f, 0.0f, cameraDistance), V3f_Create(0.0f, 0.0f, 0.0f), V3f_Create(0.0f, 1.0f, 0.0f));

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
    float elp = mgdl_GetElapsedSeconds();
    glRotatef(elp * sceneRotation.x * 10.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(elp * sceneRotation.y * 10.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(elp * sceneRotation.z * 10.0f, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, scale.z);

    Scene_Draw(scene);

    glPopMatrix();
    glDisable(GL_DEPTH_TEST);

}


#if 0

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


#endif
static const char* names[] ={ "A", "B", "+", "-", "1", "2" };
void Example::DrawInputInfo()
{
    int x = 10;
    int y = mgdl_GetScreenHeight() - 10;


    Menu_SetActive(controllerMenu);
    Menu_StartInput(x, y, 100, cursorPos, false, false);

    Menu_Text("Input");



    // Draw button states
    int buttons[] = {
        WiiButtons::ButtonA,
        WiiButtons::ButtonB,
        WiiButtons::ButtonPlus,
        WiiButtons::ButtonMinus,
        WiiButtons::Button1,
        WiiButtons::Button2
        };

    for(int i = 0; i < 6;i++ )
    {
        bool held = WiiController_ButtonHeld(mgdl_GetController(0), buttons[i]);
        Menu_Flag(names[i], held);
    }

    // DrawDPad();
    // DrawJoystick();
}

void Example::DrawTimingInfo()
{
    int x = 10;
    int y= mgdl_GetScreenHeight()-10;

    Menu_SetActive(performanceMenu);
    Menu_Start(x, y, 128);

    Menu_TextF("Deltatime %.4f", deltaTime);
    Menu_TextF("Elapsed seconds: %.2f", elapsedSeconds);

    if (sampleMusic != nullptr)
    {
        Menu_TextF("Music elapsed: %.2f", Music_GetElapsedSeconds(sampleMusic));
        SoundStatus musicStatus = Music_GetStatus(sampleMusic);
        gdl::rgba8 musicColor = gdl::Colors::Red;
        gdl::IconSymbol icon = gdl::IconSymbol::Dot;

        switch(musicStatus)
        {
            case SoundStatus::Playing:
                musicColor = gdl::Colors::Green;
                icon = gdl::IconSymbol::TriangleRight;
                break;
            case SoundStatus::Paused:
                musicColor = gdl::Colors::Yellow;
                icon = gdl::IconSymbol::TriangleVertical;
                break;
            case SoundStatus::Stopped: musicColor = gdl::Colors::Red;
                icon = gdl::IconSymbol::BlockUnder;
                break;
            case SoundStatus::Initial: musicColor = gdl::Colors::Black; break;
        };
        Menu_Icon(icon, musicColor);
    }

    float blipElapsed = Sound_GetElapsedSeconds(blip);
    Menu_TextF("Sound elapsed: %.2f", blipElapsed);
    SoundStatus musicStatus = Sound_GetStatus(blip);
    gdl::rgba8 musicColor = gdl::Colors::Red;
    gdl::IconSymbol icon = gdl::IconSymbol::Dot;
    switch(musicStatus)
    {
        case SoundStatus::Playing:
            musicColor = gdl::Colors::Green;
            icon = gdl::IconSymbol::TriangleRight;
            break;
        case SoundStatus::Paused:
            musicColor = gdl::Colors::Yellow;
            icon = gdl::IconSymbol::TriangleVertical;
            break;
        case SoundStatus::Stopped: musicColor = gdl::Colors::Red;
            icon = gdl::IconSymbol::BlockUnder;
            break;
        case SoundStatus::Initial: musicColor = gdl::Colors::Black; break;
    };
    Menu_Icon(icon, musicColor);
}

void Example::DrawMenu()
{
    int w = 164;
    int x = mgdl_GetScreenWidth() - w;
    int y = mgdl_GetScreenHeight() - 8;

    Menu_SetActive(menu);
    Menu_Start(x, y, w);

    Menu_Text("Toggle features");
    Menu_Toggle("Sprites", &toggleSprites);
    Menu_Toggle("3D", &toggle3D);
    Menu_Toggle("Image", &toggleImage);
    Menu_Toggle("Camera", &toggleCamera);
    Menu_Toggle("Inputs", &toggleInputs);
    Menu_Toggle("Performance", &togglePerformance);
    Menu_Toggle("Audio", &toggleAudio);
}

void Example::DrawAudio()
{
    Menu_SetActive(audioMenu);
    Menu_Start(10, mgdl_GetScreenHeight()-10, 64);

    if (Menu_Button("Play Ogg"))
    {
        Music_Play(sampleMusic, false);
    }
    if (Menu_Button("Pause Ogg"))
    {
        bool ispaused = Music_GetStatus(sampleMusic) == SoundStatus::Paused;
        Music_SetPaused(sampleMusic, !ispaused);
    }
    if (Menu_Button("Stop Ogg"))
    {
        Music_Stop(sampleMusic);
    }
    if (Menu_Toggle("Loop Ogg", &musicLooping ))
    {
        Music_SetLooping(sampleMusic, musicLooping);
    }
    if (Menu_Button("Play Sound"))
    {
        Sound_Play(blip);
    }
}
#if 0
#endif

void Example::DrawCameraControls()
{
    int x = 10;
    int y = mgdl_GetScreenHeight() - 10;
    int w = 64;

    Menu_SetActive(cameraMenu);
    Menu_Start(x, y, w);

    Menu_Text("Control camera");
    if (Menu_Button("Closer!"))
    {
        cameraDistance -= 1.0f;
    }
    if (Menu_Button("Away!"))
    {
        cameraDistance += 1.0f;
    }
    if (Menu_Button("Rotate Left"))
    {
        sceneRotation.y -= 1.0f;
    }
    if (Menu_Button("Rotate right!"))
    {
        sceneRotation.y += 1.0f;
    }
    if (Menu_Button("Rotate Up"))
    {
        sceneRotation.x -= 1.0f;
    }
    if (Menu_Button("Rotate Down!"))
    {
        sceneRotation.x += 1.0f;
    }
    if (Menu_Button("Rotate CW"))
    {
        sceneRotation.z -= 1.0f;
    }
    if (Menu_Button("Rotate CCW!"))
    {
        sceneRotation.z += 1.0f;
    }
    if (Menu_Button("Reset "))
    {
        sceneRotation = V3f_Create(0,0,0);
    }
}

