#pragma once

#include <mgdl.h>

class Example
{
    Image* barb;
    Sprite* mel_sprites;
    Sprite* fruitSprites;
    Image* pointerImage;
    Font* ibmFont;
    Font* debugFont;
    Music* sampleMusic;
    Sound* blip;
    Menu* menu;
    Menu* cameraMenu;
    Menu* controllerMenu;
    Menu* performanceMenu;
    Menu* audioMenu;
    Scene* wiiScene;
    Scene* shipScene;
    Image* wiiTexture;

    Image* matcapTexture;
    Image* checkerTexture;
    Scene* icosaScene;
    Material* matcapMaterial;

    float cameraDistance = 1.0f;
    vec3 sceneRotation;

    float deltaTime;
    float elapsedSeconds;

    // Toggles
    bool toggleSprites, toggle3D, toggleImage, toggleCamera, toggleInputs, togglePerformance, toggleAudio = false;

    public:
        Example();
        void Init();
        void Update();
        void Draw();

    private:
        void DrawMenu();

        // Togglable
        void DrawSprites();
        void DrawIcosa();
        void DrawImage();
        void DrawCameraControls();
        void DrawInputInfo();
        void DrawTimingInfo();
        void DrawAudio();

        void DrawVersion();
        void DrawScene(Scene* scene, vec3 scale);

        vec2 cursorPos;
        bool mouseClick;
        bool mouseDown;
        bool musicLooping;

};
