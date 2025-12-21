
#include "example.h"
#include <mgdl/mgdl-draw2d.h>

#include <string>

#ifdef MGDL_ROCKET
    #include <mgdl/mgdl-rocket.h>

    #ifdef SYNC_PLAYER
        #include MGDL_ROCKET_FILE_H
    #else
        static ROCKET_TRACK sync_value;
    #endif
#endif

Example::Example()
{

}

void Example::Init()
{
    // Sprites, images and fonts
    short spriteHeight = 64;
    barb = mgdl_LoadTexture("assets/barb.png", TextureFilterModes::Linear);
    mel_sprites = mgdl_LoadSprite("assets/mel_tiles.png", spriteHeight, spriteHeight);
    fruitSprites = mgdl_LoadSprite("assets/fruits.png", 16, 16);
    pointerTexture = mgdl_LoadTexture("assets/pointer.png", TextureFilterModes::Nearest);

    ibmFont = mgdl_LoadFont("assets/font8x16.png", 8, 16, ' ');
    debugFont = DefaultFont_GetDefaultFont();

    // Audio
    blip = mgdl_LoadSoundWav("assets/blipSelect.wav");
    sampleMusic = mgdl_LoadSoundOgg("assets/sample3.ogg");

    // Wii model scene
    wiiScene = mgdl_LoadFBX("assets/wii_et_baby.fbx");
    wiiTexture = mgdl_LoadTexture("assets/wii_console_texture.png", TextureFilterModes::Nearest);
    Scene_SetMaterialTexture(wiiScene, "wii_console_texture.png", wiiTexture);

    // Ship with matcap texture
    shipScene = mgdl_LoadFBX("assets/ship_with_uvs.fbx");
    matcapTexture = mgdl_LoadTexture("assets/matcap.png", TextureFilterModes::Linear);
    matcapMaterial = Material_Load("matcap", matcapTexture, MaterialType::Matcap);
    Scene_SetAllMaterialTextures(shipScene, matcapTexture);
    Material* st = Scene_GetMaterial(shipScene, "standardSurface1");
    Material* mt2 = Scene_GetMaterial(shipScene, "Material.002");
    if (st!=nullptr)
    {
        st->type = MaterialType::Matcap;
    }
    if (mt2!=nullptr)
    {
        mt2->type = MaterialType::Matcap;
    }

    // Generated icosahedron and checkerboard texture
    icosaScene = Scene_CreateEmpty();
    checkerTexture = Texture_GenerateCheckerBoard();
    Material* checkerMaterial = Material_Load("checker", checkerTexture, MaterialType::Diffuse);

    Scene_AddMaterial(icosaScene, checkerMaterial );
    Mesh* quad = Mesh_CreateQuad(FlagNormals | FlagUVs);
    // Mesh* icosaMesh = Mesh_CreateIcosahedron(FlagNormals | FlagUVs);
    Node* icosaNode = Node_Create(1);
    Node_SetContent(icosaNode, "icosaNode", quad, checkerMaterial);
    Scene_AddChildNode(icosaScene, nullptr, icosaNode);


    menu = Menu_CreateWindowed(ibmFont, 1.0f, 1.0f, 128, 256, "MTEK GDL");
    cameraMenu = Menu_CreateWindowed(debugFont, 2.0f, 1.0f, 128, 256, "Camera");
    controllerMenu = Menu_CreateWindowed(ibmFont, 1.0f, 1.0f, 128, 356, "Controls");
    performanceMenu = Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 256, 64, "Performance");
    audioMenu = Menu_CreateWindowed(debugFont, 2.0f, 1.0f, 256, 256, "Audio");

    if (sampleMusic)
    {
        musicLooping = Sound_GetLooping(sampleMusic);
    }
    sceneRotation = V3f_Create(0.0f, 1.0f,0.0f);
    //quad->DebugPrint();

    cameraDistance = 30.0f;


    #ifdef MGDL_ROCKET
        // Connect to editor
        RocketTrackFormat trackSource = TrackEditor;
        #ifdef SYNC_PLAYER
            // Read from CPP file
            RocketTrackFormat trackSource = TrackCPP;
        #endif

        bool rocketInit = Rocket_Connect(trackSource, TrackCPP, sampleMusic, 120, 4);
        if (rocketInit == false)
        {
            mgdl_DoProgramExit();
        }

        #ifdef SYNC_PLAYER
            // In release mode start the music and tracks
            Rocket_PlayTracks();
        #else
            // When tracks are in CPP file, this should not be called
            sync_value = Rocket_AddTrack("sync_value");
        #endif
    #endif
}

void Example::Quit()
{
#ifdef MGDL_ROCKET
    Rocket_Disconnect();
#endif
}

void Example::Update()
{
    #ifdef MGDL_ROCKET

        Rocket_UpdateRow();
        float r = Rocket_Float(sync_value);

        #ifndef SYNC_PLAYER

            if (WiiController_ButtonPress(mgdl_GetController(0), WiiButtons::Button2))
            {
                Rocket_SaveAllTracks();
            }
        #endif
    #endif

    elapsedSeconds = mgdl_GetElapsedSeconds();
    deltaTime = mgdl_GetDeltaTime();

    cursorPos = WiiController_GetCursorPosition(Platform_GetController(0));
    mouseClick = WiiController_ButtonPress(Platform_GetController(0), ButtonA);
    mouseDown = WiiController_ButtonHeld(Platform_GetController(0), ButtonA);

    //Music_UpdatePlay(sampleMusic);
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

void DrawTextDouble(const char* text, short x, short y, float textHeight, Font* font)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        font->Print(Colors::Black, textHeight, LJustify, LJustify, text);

        glTranslatef(-1, -1, 0.0f);
        font->Print(Colors::LightGreen, textHeight, LJustify, LJustify, text);
    glPopMatrix();
}
#endif

void Example::Draw()
{
    Color4f black = Palette_GetColor4f(Palette_GetDefault(), 5);
    mgdl_glClearColor4f(&black);

    mgdl_InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( toggle3D) {DrawIcosa();}

    mgdl_InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( toggleSprites) {DrawSprites();}
    if ( toggleTexture) {DrawTexture();}
    if ( toggleCamera) {DrawCameraControls();}
    if ( toggleInputs) {DrawInputInfo();}
    if ( togglePerformance) {DrawTimingInfo();}
    if ( toggleAudio) {DrawAudio();}

    DrawMenu();
}

void Example::DrawSprites()
{
    mgdl_glSetAlphaTest(true);
    mgdl_glSetTransparency(true);
    for (int i = 0; i < 16; i++)
    {
        int size = 64;
        Sprite_Draw2D(fruitSprites, i, size * (i%4), size + (i/4) * size, size, LJustify, RJustify, Color_GetDefaultColor(Color_White));
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
        Sprite_Draw2D(mel_sprites, i, placeX, placeY, spriteH, LJustify, LJustify, Color_GetDefaultColor(Color_White));
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

void Example::DrawTexture()
{
    // Draw Texture
    Texture_Draw2DAligned(barb,
            0,
            mgdl_GetScreenHeight()/2,
            1.0f,
            LJustify, Centered);

    Texture_Draw2DAligned(debugFont->_fontTexture,
            0,
            mgdl_GetScreenHeight()/2,
            1.0f,
            LJustify, Centered);
}

void Example::DrawScene ( Scene* scene, V3f scale)
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



void DrawDPad(short x, short y, short size)
{

    short box = size;
    short h=box/2;
    y -= box + h;
    // Dpad
    int dpad_buttons[] = {
        WiiButtons::ButtonUp,
        WiiButtons::ButtonDown,
        WiiButtons::ButtonLeft,
        WiiButtons::ButtonRight
    };
    vec2 directions[] = {
        vec2New(0,1),
        vec2New(0,-1),
        vec2New(-1,0),
        vec2New(1,0)
    };
    Palette* blessing = Palette_GetDefault();
    Color4f c = Palette_GetColor4f(blessing, 5);

    //Draw2D_Line(x, y, x+size, y-size, &c);

    for (int i=0;i<4;i++)
    {
        if (WiiController_ButtonHeld(mgdl_GetController(0), dpad_buttons[i]))
        {
            c = Palette_GetColor4f(blessing, 2);
        }
        else
        {
            c = Palette_GetColor4f(blessing, 5);
        }
        vec2 d=directions[i];
        Draw2D_Rect(x+d.x*box-h, y+ d.y*box-h, x+box+d.x*box-h, y+box+d.y*box-h, &c);
    }
}

void DrawJoystick(short x, short y, short size)
{
    // Draw joystick direction
    short jsize=size;
    short box = jsize;
    short h=box/2;
    Palette* blessing = Palette_GetDefault();
    Color4f jc = Palette_GetColor4f(blessing, 5);
    vec2 jdir = WiiController_GetNunchukJoystickDirection(mgdl_GetController(0));
    short jleft= x + jsize/2 + jdir.x * box-h;
    short jtop = y - jsize/2 - jdir.y * box-h;
    Draw2D_RectLines(x-jsize, y, x+jsize*2, y-jsize*3, &jc);
    if (jdir.x != 0.0f || jdir.y != 0.0f)
    {
        jc = Palette_GetColor4f(blessing, 2);
    }
    Draw2D_Rect(jleft, jtop, jleft+box, jtop-box, &jc);
}


void Example::DrawInputInfo()
{
    int x = 10;
    int y = mgdl_GetScreenHeight() - 10;


    Menu_StartInput(controllerMenu, x, y, 100, cursorPos, false, false);

    Menu_Text(controllerMenu, "Buttons");

    // Draw button states
    int buttons[8] = {
        WiiButtons::ButtonA,
        WiiButtons::ButtonB,
        WiiButtons::ButtonPlus,
        WiiButtons::ButtonMinus,
        WiiButtons::Button1,
        WiiButtons::Button2,
        WiiButtons::ButtonZ,
        WiiButtons::ButtonC
        };

    for(int i = 0; i < 8;i++ )
    {
        bool held = WiiController_ButtonHeld(mgdl_GetController(0), buttons[i]);
        Menu_Flag(controllerMenu, WiiController_GetButtonSymbol(buttons[i]), held);
    }

    Menu_Text(controllerMenu, "D pad");
    DrawDPad(controllerMenu->drawx + 50, controllerMenu->drawy, controllerMenu->textSize);
    Menu_Skip(controllerMenu, controllerMenu->textSize*3);

    Menu_Text(controllerMenu, "Joystick");
    DrawJoystick(controllerMenu->drawx + 50, controllerMenu->drawy, controllerMenu->textSize);
    Menu_Skip(controllerMenu, controllerMenu->textSize*3);

    float pitch = WiiController_GetPitch(mgdl_GetController(0));
    float yaw = WiiController_GetYaw(mgdl_GetController(0));
    float roll = WiiController_GetRoll(mgdl_GetController(0));
    Menu_TextF(controllerMenu, "Pitch %.0f ", Rad2Deg(pitch));
    Menu_TextF(controllerMenu, "Yaw %.0f", Rad2Deg(yaw));
    Menu_TextF(controllerMenu, "Roll %.0f", Rad2Deg(roll));
}

void Example::DrawTimingInfo()
{
    int x = 10;
    int y= mgdl_GetScreenHeight()-10;

    Menu_Start(performanceMenu, x, y, 128);

    Menu_TextF(performanceMenu, "Deltatime %.4f", deltaTime);
    Menu_TextF(performanceMenu, "Elapsed seconds: %.2f", elapsedSeconds);

}

void Example::DrawMenu()
{
    int w = 164;
    int x = mgdl_GetScreenWidth() - w;
    int y = mgdl_GetScreenHeight() - 8;

    Menu_Start(menu, x, y, w);

    Menu_Text(menu, "Toggle features");
    Menu_Toggle(menu, "Sprites", &toggleSprites);
    Menu_Toggle(menu, "3D", &toggle3D);
    Menu_Toggle(menu, "Texture", &toggleTexture);
    Menu_Toggle(menu, "Camera", &toggleCamera);
    Menu_Toggle(menu, "Inputs", &toggleInputs);
    Menu_Toggle(menu, "Performance", &togglePerformance);
    Menu_Toggle(menu, "Audio", &toggleAudio);
#if MGDL_ROCKET
    if (Menu_Button(menu, "Write Rocket"))
    {
        Rocket_SaveAllTracks();
    }
#endif
    Menu_DrawCursor(menu);
}

void Example::DrawSoundStatus(mgdlAudioStateEnum status)
{

    Color4f* musicColor = Color_GetDefaultColor(Color_Red);
    IconSymbol icon = IconSymbol::Icon_Dot;
    switch(status)
    {
        case Audio_StatePlaying:
            musicColor = Color_GetDefaultColor(Color_Green);
            icon = IconSymbol::Icon_TriangleUp;
            Menu_Text(audioMenu, "Playing");
            break;
        case Audio_StatePaused:
            musicColor = Color_GetDefaultColor(Color_White);
            icon = IconSymbol::Icon_Clock;
            Menu_Text(audioMenu, "Paused");
            break;
        case Audio_StateStopped:
            musicColor = Color_GetDefaultColor(Color_Red);
            icon = IconSymbol::Icon_Skull;
            Menu_Text(audioMenu, "Stopped");
            break;
        case Audio_StateInvalid:
            musicColor = Color_GetDefaultColor(Color_Black);
            Menu_Text(audioMenu, "Invalid");
        break;
    };
    Menu_Icon(audioMenu, icon, musicColor);
}

void Example::DrawAudio()
{
    Menu_Start(audioMenu, 10, mgdl_GetScreenHeight()-10, 256);

    if (Menu_Button(audioMenu, "Play Ogg"))
    {
        Audio_PlaySound(sampleMusic);
    }
    if (Menu_Button(audioMenu, "Pause Ogg"))
    {
        bool ispaused = Audio_GetSoundStatus(sampleMusic) == Audio_StatePaused;
        Audio_PauseSound(sampleMusic);
    }
    if (Menu_Button(audioMenu, "Stop Ogg"))
    {
        Audio_StopSound(sampleMusic);
    }
    if (Menu_Toggle(audioMenu, "Loop Ogg", &musicLooping ))
    {
        // Music_SetLooping(sampleMusic, musicLooping);
    }
    if (Menu_Button(audioMenu, "Play Sound"))
    {
        Audio_PlaySound(blip);
    }
    if (sampleMusic != nullptr)
    {
        Menu_TextF(audioMenu, "Music elapsed: %.2f", Sound_GetElapsedSeconds(sampleMusic));
        mgdlAudioStateEnum musicStatus = Audio_GetSoundStatus(sampleMusic);
        DrawSoundStatus(musicStatus);

    }

    u32 blipElapsed = Audio_GetSoundElapsedMs(blip);
    Menu_TextF(audioMenu, "Sound elapsed: %.2f", blipElapsed/1000.0f);
    mgdlAudioStateEnum soundStatus = Audio_GetSoundStatus(blip);
    DrawSoundStatus(soundStatus);
}
#if 0
#endif

void Example::DrawCameraControls()
{
    int x = 10;
    int y = mgdl_GetScreenHeight() - 10;
    int w = 64;

    Menu_Start(cameraMenu, x, y, w);

    Menu_Text(cameraMenu, "Control camera");
    if (Menu_Button(cameraMenu, "Closer!"))
    {
        cameraDistance -= 1.0f;
    }
    if (Menu_Button(cameraMenu, "Away!"))
    {
        cameraDistance += 1.0f;
    }
    if (Menu_Button(cameraMenu, "Rotate Left"))
    {
        sceneRotation.y -= 1.0f;
    }
    if (Menu_Button(cameraMenu, "Rotate right!"))
    {
        sceneRotation.y += 1.0f;
    }
    if (Menu_Button(cameraMenu, "Rotate Up"))
    {
        sceneRotation.x -= 1.0f;
    }
    if (Menu_Button(cameraMenu, "Rotate Down!"))
    {
        sceneRotation.x += 1.0f;
    }
    if (Menu_Button(cameraMenu, "Rotate CW"))
    {
        sceneRotation.z -= 1.0f;
    }
    if (Menu_Button(cameraMenu, "Rotate CCW!"))
    {
        sceneRotation.z += 1.0f;
    }
    if (Menu_Button(cameraMenu, "Reset "))
    {
        sceneRotation = V3f_Create(0,0,0);
    }
}

