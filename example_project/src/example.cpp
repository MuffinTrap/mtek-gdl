
#include "example.h"
#include <mgdl/mgdl-script-api.h>
#include <string>

#if defined(MGDL_ROCKET)
    #include <mgdl-rocket.h>
    static ROCKET_TRACK sync_value;
#endif

#if defined(USE_ANGEL_AS_CPP)
#include <angel.hpp>
#endif

Example::Example()
{

}

void Example::AngelInit()
{
    // AngelScript
#if defined(USE_ANGEL_AS_SCRIPT)
    angelContext = mgdl_InitAngelScript("scripts/angel.cpp", "scripts", "example");
    if (angelContext != nullptr)
    {
        mgdl_RunAngelScriptInit(angelContext);
    }
#elif defined(USE_ANGEL_AS_CPP)
    angelContext = mgdl_InitAngelCpp(&angelscript_init, &angelscript_frame, &angelscript_quit);
	mgdl_RunAngelScriptInit(angelContext);
#endif
    AssetManager_PrintLoadedTextures();
}

void Example::AngelFrame()
{
    mgdl_RunAngelScriptFrame(angelContext, mgdl_GetDeltaTime());
}

void Example::Init()
{
    Log_SaveLines(256);

    // Sprites, images and fonts
    barb = mgdl_LoadTexture("assets/barb.png");
    mgdl_SetTextureFilter(barb, TextureFilterModes::Linear);
    short spriteHeight = 64;
    mel_sprites = mgdl_LoadTexture("assets/mel_tiles.png");
    mgdl_CreateSpriteAtlas(mel_sprites, spriteHeight, spriteHeight);
    fruitSprites = mgdl_LoadTexture("assets/fruits.png");
    mgdl_CreateSpriteAtlas(fruitSprites, 16, 16);

    ibmFont = mgdl_LoadTexture("assets/font8x16.png");
    mgdl_CreateFont(ibmFont, 8, 16, ' ');

    /*

    // Wii model scene
    // wiiScene = mgdl_LoadFBX("assets/wii_et_baby.fbx");
    wiiTexture = mgdl_LoadTexture("assets/wii_console_texture.png");

    // TODO Materials somewhere else than in scene
    // Scene_SetMaterialTexture(wiiScene, "wii_console_texture.png", wiiTexture);

    // Ship with matcap texture
    shipScene = mgdl_LoadFBX("assets/ship_with_uvs.fbx");
    matcapTexture = mgdl_LoadTexture("assets/matcap.png");
    matcapMaterial = Material_Load("matcap", AssetManager_GetTexture(matcapTexture), MaterialType::Matcap);
    // TODO Scene_SetAllMaterialTextures(shipScene, matcapTexture);
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
    */


    Texture* debugFont = DefaultFont_GetDefaultFont();
    menu =              Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 128, 256, "MTEK GDL");
    cameraMenu =        Menu_CreateWindowed(debugFont, 2.0f, 1.0f, 128, 256, "Camera");
    controllerMenu =    Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 128, 356, "Controls");
    performanceMenu =   Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 256, 64, "Performance");
    audioMenu =         Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 256, 256, "Audio");
    logMenu =           Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 620, 256, "Log");
    rocketMenu =        Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 256, 356, "Rocket");
    angelMenu =        Menu_CreateWindowed(debugFont, 1.0f, 1.0f, 256, 32, "AngelScript");

    if (sampleMusic)
    {
        musicLooping = mgdl_GetBool(sampleMusic, MGDL_SOUND_LOOPING);
    }
    sceneRotation = Vector3New(0.0f, 1.0f,0.0f);
    //quad->DebugPrint();

    cameraDistance = 30.0f;


    // Audio
    blip = mgdl_LoadSound("assets/blipSelect.wav");
    sampleMusic = mgdl_LoadSound("assets/sample3.ogg");
    testmp3Music = mgdl_LoadSound("assets/test_jam.mp3");
    /*

    #ifdef MGDL_ROCKET

        RocketTrackFormat source = Rocket_Connect(TrackJSON, sampleMusic, 120, 4);
        if (source == TrackInvalid)
        {
            mgdl_DoProgramExit();
        }
        // Load all tracks
        sync_value = Rocket_AddTrack("sync_value");

        if (source != TrackEditor)
        {
            // Start the music if no editor was found
            Rocket_PlayTracks();
        }
    #endif

    */
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
        if (WiiController_ButtonPress(mgdl_GetController(0), WiiButtons::Button2))
        {
            Rocket_SaveAllTracks();
        }
    #endif

    elapsedSeconds = mgdl_GetElapsedSeconds();
    deltaTime = mgdl_GetDeltaTime();

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
    color32 black = Palette_GetColor(Palette_GetDefault(), 5);
    mgdl_glClearColor32(black);

    mgdl_InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( toggle3D) {DrawIcosa();}

    mgdl_InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    if (toggleLog) { DrawLog();}
    if ( toggleSprites) {DrawSprites();}
    if ( toggleTexture) {DrawTexture();}
    if ( toggleCamera) {DrawCameraControls();}
    if ( toggleInputs) {DrawInputInfo();}
    if ( togglePerformance) {DrawTimingInfo();}
    if ( toggleAudio) {DrawAudio();}
    if ( toggleRocket) {DrawRocket();}
    if ( toggleAngel) {DrawAngel();}

    DrawMenu();
}

void Example::DrawSprites()
{
    mgdl_glSetAlphaTest(true);
    mgdl_glSetTransparency(true);
    int size = mgdl_GetInt(fruitSprites, MGDL_SPRITE_WIDTH) * 2;
    for (int i = 0; i < 16; i++)
    {
        mgdl_DrawSpriteEx(fruitSprites, i, size * (i%4), size + (i/4) * size, 2.0f, Debug_White);
    }

    const short h = mgdl_GetInt(mel_sprites, MGDL_SPRITE_WIDTH);
    const short w = mgdl_GetInt(mel_sprites, MGDL_SPRITE_HEIGHT);
    float scale = 2.0f;
    short spriteW = w * scale;
    short spriteH = h * scale;
    short placeX = mgdl_GetScreenWidth() - spriteW;
    short placeY = mgdl_GetScreenHeight();
    for (short i = 0; i < 4; i++)
    {
        mgdl_DrawSpriteEx(mel_sprites, i, placeX, placeY, scale, Debug_White);
        placeY -= spriteH;
    }
}

void Example::DrawIcosa()
{
    cameraDistance = 5.0f;
    DrawScene(icosaScene, Vector3New(1.0f, 1.0f, 1.0f));
    cameraDistance = 15.0f;
    DrawScene(wiiScene, Vector3New(0.1f, 0.1f, 0.1f));
}

void Example::DrawTexture()
{
    Vector2 pos = mgdl_CalculateAlignedTopLeft(mgdl_GetScreenWidth()/2, mgdl_GetScreenHeight()/2, mgdl_GetInt(barb, MGDL_TEXTURE_WIDTH), mgdl_GetInt(barb, MGDL_TEXTURE_HEIGHT), Centered, Centered);
    mgdl_DrawTexture(barb, pos.x, pos.y);
}

void Example::DrawScene ( Scene* scene, Vector3 scale)
{
    // Try to draw Wii 3D model
    mgdl_InitPerspectiveProjection(75.0f, 0.1f, 100.0f);
    mgdl_InitCameraF(0.0f, 0.0f, cameraDistance,
                     0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f);

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

    Scene_DrawFbx(scene);

    glPopMatrix();
    glDisable(GL_DEPTH_TEST);

}



void DrawDPad(short x, short y, short size)
{
    short box = size;
    short h=box/2;
    x = x + h;
    y = y - h;
    // Dpad
    int dpad_buttons[] = {
        WiiButtons::ButtonUp,
        WiiButtons::ButtonDown,
        WiiButtons::ButtonLeft,
        WiiButtons::ButtonRight
    };
    Vector2 directions[] = {
        Vector2New(0,1),
        Vector2New(0,-1),
        Vector2New(-1,0),
        Vector2New(1,0)
    };
    Palette* blessing = Palette_GetDefault();
    color32 c = Palette_GetColor(blessing, 5);

    //Draw2D_Line(x, y, x+size, y-size, &c);

    for (int i=0;i<4;i++)
    {
        if (WiiController_ButtonHeld(mgdl_GetController(0), dpad_buttons[i]))
        {
            c = Palette_GetColor(blessing, 2);
        }
        else
        {
            c = Palette_GetColor(blessing, 5);
        }
        Vector2 d=directions[i];
        mgdl_DrawRectangle(x + d.x * box-h,
                    y + d.y * box-h,
                    box,
                    box,
                    c);
    }
}

void DrawJoystick(short x, short y, short size)
{
    // Draw joystick direction
    short jsize=size;
    short box = jsize;
    short h=box/2;
    Palette* blessing = Palette_GetDefault();
    color32 jc = Palette_GetColor(blessing, 5);
    Vector2 jdir = WiiController_GetNunchukJoystickDirection(mgdl_GetController(0));
    short jleft= x + jsize/2 + jdir.x * box-h;
    short jtop = y - jsize/2 - jdir.y * box-h;
    // Lined Rectangle shows limits
    mgdl_DrawRectangleLines(x-jsize, y, box*3, box*3, jc);
    if (jdir.x != 0.0f || jdir.y != 0.0f)
    {
        jc = Palette_GetColor(blessing, 2);
    }
    // Filled rectangle shows position
    mgdl_DrawRectangle(jleft, jtop, box, box, jc);
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
    Menu_Toggle(menu, "Log", &toggleLog);
    Menu_Toggle(menu, "AngelScript", &toggleAngel);
#if MGDL_ROCKET

    Menu_Toggle(menu, "Rocket", &toggleRocket);
#endif
    Menu_DrawCursor(menu);
}

void Example::DrawSoundStatus(mgdlAudioStateEnum status)
{
    color32 musicColor = Debug_Red;
    IconSymbol icon = IconSymbol::Icon_Dot;
    switch(status)
    {
        case Audio_StatePlaying:
            musicColor = Debug_Green;
            icon = IconSymbol::Icon_TriangleUp;
            Menu_Text(audioMenu, "Playing");
            break;
        case Audio_StatePaused:
            musicColor = Debug_White;
            icon = IconSymbol::Icon_Clock;
            Menu_Text(audioMenu, "Paused");
            break;
        case Audio_StateStopped:
            musicColor = Debug_Red;
            icon = IconSymbol::Icon_Skull;
            Menu_Text(audioMenu, "Stopped");
            break;
        case Audio_StateInvalid:
            musicColor = Debug_Black;
            Menu_Text(audioMenu, "Invalid");
        break;
    };
    Menu_Icon(audioMenu, icon, musicColor);
}

void Example::DrawLog()
{
    Menu_Start(logMenu, 10, mgdl_GetScreenHeight()/2+48, 256);
    int amount = 32;
    for(int i = amount; i >= 0; i--)
    {
        Menu_Text(logMenu, Log_GetLastLine(i));
    }

}

void Example::DrawAudio()
{
    Menu_Start(audioMenu, 10, mgdl_GetScreenHeight()-10, 128);

    if (Handle_IsValid(testmp3Music))
    {
        // MP3 music
        if (Menu_Button(audioMenu, "Play Mp3"))
        {
            mgdl_PlaySound(testmp3Music);
        }
        bool paused = mgdl_GetBool(testmp3Music, MGDL_SOUND_PAUSED);
        if (!paused)
        {
            if (Menu_Button(audioMenu, "Pause Mp3"))
            {
                mgdl_PauseSound(testmp3Music);
            }
        }
        if (Menu_Button(audioMenu, "Stop Mp3"))
        {
            mgdl_StopSound(testmp3Music);
        }
            Menu_TextF(audioMenu, "Music elapsed: %.2f", mgdl_GetInt(testmp3Music, MGDL_SOUND_ELAPSED_MS)/1000.0f);
            mgdlAudioStateEnum musicStatus = (mgdlAudioStateEnum)mgdl_GetInt(testmp3Music, MGDL_SOUND_STATUS_ENUM);
            DrawSoundStatus(musicStatus);
    }
    if (Handle_IsValid(sampleMusic))
    {
        // MP3 music
        if (Menu_Button(audioMenu, "Play Ogg"))
        {
            mgdl_PlaySound(sampleMusic);
        }
        bool paused = mgdl_GetBool(sampleMusic, MGDL_SOUND_PAUSED);
        if (!paused)
        {
            if (Menu_Button(audioMenu, "Pause Ogg"))
            {
                mgdl_PauseSound(sampleMusic);
            }
        }
        if (Menu_Button(audioMenu, "Stop Ogg"))
        {
            mgdl_StopSound(sampleMusic);
        }
            Menu_TextF(audioMenu, "Music elapsed: %.2f", mgdl_GetInt(sampleMusic, MGDL_SOUND_ELAPSED_MS)/1000.0f);
            mgdlAudioStateEnum musicStatus = (mgdlAudioStateEnum)mgdl_GetInt(sampleMusic, MGDL_SOUND_STATUS_ENUM);
            DrawSoundStatus(musicStatus);
    }


    if (Menu_Button(audioMenu, "Play Sound"))
    {
        mgdl_PlaySound(blip);
    }
    u32 blipElapsed = mgdl_GetInt(blip, MGDL_SOUND_ELAPSED_MS);
    Menu_TextF(audioMenu, "Sound elapsed: %.2f", blipElapsed/1000.0f);
    mgdlAudioStateEnum soundStatus = (mgdlAudioStateEnum)mgdl_GetInt(blip, MGDL_SOUND_STATUS_ENUM);
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
        sceneRotation = Vector3New(0,0,0);
    }
}

void Example::DrawRocket()
{
    Menu_Start(rocketMenu, 10, mgdl_GetScreenHeight()-10, 256);
#ifdef MGDL_ROCKET
    if (Menu_Button(rocketMenu, "Write Rocket"))
    {
        Rocket_SaveAllTracks();
    }
    Menu_Text(rocketMenu, "Track values:");
    for (u16 i = 0; i < Rocket_GetTrackAmount(); i++)
    {
        ROCKET_TRACK t = Rocket_GetTrack(i);
        if (t != nullptr)
        {
            Menu_TextF(rocketMenu, "%s: %.2f", t->name, Rocket_Float(t));
        }
        else
        {
            break;
        }
    }
#else
    Menu_Text(rocketMenu, "MGDL_ROCKET not defined");

#endif
}
void Example::DrawAngel()
{
    Menu_Start(angelMenu, 10, mgdl_GetScreenHeight()-10, 256);
}


