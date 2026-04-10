#pragma once

#include <mgdl.h>
#include <angelscript.h>
#include <scriptbuilder.h>

class Example
{
    TextureHandle barb;
    Sprite* mel_sprites;
    Sprite* fruitSprites;
    Font* ibmFont;
    Font* debugFont;
    Sound* sampleMusic;
    Sound* blip;
    Sound* testmp3Music;
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
    V3f sceneRotation;

    float deltaTime;
    float elapsedSeconds;

    // Scripting
    asIScriptEngine *as_engine;
    asIScriptContext* as_ctx;
    asIScriptFunction* as_mainFunc;

    // Toggles
    bool toggleSprites, toggle3D, toggleTexture, toggleCamera, toggleInputs, togglePerformance, toggleAudio, toggleLog, toggleRocket, toggleAngel = false;

    public:
        Example();
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
        void DrawScene(Scene* scene, V3f scale);

        vec2 cursorPos;
        bool mouseClick;
        bool mouseDown;
        bool musicLooping;

};
