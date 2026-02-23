#pragma once

#include <mgdl.h>

class Example
{
    Texture* barb;
    Sprite* mel_sprites;
    Sprite* fruitSprites;
    Texture* pointerTexture;
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
    Scene* wiiScene;
    Scene* shipScene;
    Texture* wiiTexture;

    Texture* matcapTexture;
    Texture* checkerTexture;
    Scene* icosaScene;
    Material* matcapMaterial;

    float cameraDistance = 1.0f;
    V3f sceneRotation;

    float deltaTime;
    float elapsedSeconds;

    // Toggles
    bool toggleSprites, toggle3D, toggleTexture, toggleCamera, toggleInputs, togglePerformance, toggleAudio, toggleLog, toggleRocket = false;

    public:
        Example();
        void Init();
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

        void DrawVersion();
        void DrawScene(Scene* scene, V3f scale);

        vec2 cursorPos;
        bool mouseClick;
        bool mouseDown;
        bool musicLooping;

};
