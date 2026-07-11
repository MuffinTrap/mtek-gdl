#pragma once

#include <mgdl.h>
#include <mgdl/mgdl-angelscript.h>

class Example
{
    TextureHandle barb;
    TextureHandle mel_sprites;
    TextureHandle fruitSprites;
    TextureHandle ibmFont;
    SoundHandle sampleMusic;
    SoundHandle blip;
    SoundHandle testmp3Music;
    Menu* menu;
    Menu* cameraMenu;
    Menu* controllerMenu;
    Menu* performanceMenu;
    Menu* audioMenu;
    Menu* logMenu;
    Menu* rocketMenu;
    Menu* angelMenu;
    Scene* wiiScene;
    Scene* shipScene;
    TextureHandle wiiTexture;

    TextureHandle matcapTexture;
    Texture* checkerTexture;
    Scene* icosaScene;
    Material* matcapMaterial;

    float cameraDistance = 1.0f;
    Vector3 sceneRotation;

    float deltaTime;
    float elapsedSeconds;

    mgdl_AngelScript* angelContext;


    // Toggles
    bool toggleSprites, toggle3D, toggleTexture, toggleCamera, toggleInputs, togglePerformance, toggleAudio, toggleLog, toggleRocket, toggleAngel = false;

    public:
        Example();

        void AngelInit();
        void AngelFrame();

        void Init();
        void InitAngelScript();
        void LoadAngelScript(const char* script);
        void ReloadAngelScript(const char* script);
        void RunAngelScript();
        void DeinitAngelScript();
        void Update();
        void Draw();
        void Quit();

    private:
        void DrawMenu();

        // Togglable
        void DrawSprites();
        void DrawIcosa();
        void DrawTexture();
        void DrawCameraControls();
        void DrawInputInfo();
        void DrawTimingInfo();
        void DrawAudio();
        void DrawLog();
        void DrawSoundStatus(mgdlAudioStateEnum status);
        void DrawRocket();
        void DrawAngel();

        void DrawVersion();
        void DrawScene(Scene* scene, Vector3 scale);

        Vector2 cursorPos;
        bool mouseClick;
        bool mouseDown;

};
