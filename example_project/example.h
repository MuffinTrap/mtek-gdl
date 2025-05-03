#pragma once

#include <mgdl.h>

class Example
{
    Image* barb;
    Sprite* mel_sprites;
    Image* pointerImage;
    Font* ibmFont;
    Font* debugFont;
    Music* sampleMusic;
    Sound* blip;
    Menu* menu;
    Menu* cameraMenu;
    Scene* wiiScene;
    Image* wiiTexture;

    Image* matcapTexture;
    Scene* icosaScene;
    Material* matcapMaterial;

    float cameraDistance = 1.0f;
    vec3 sceneRotation;

    float deltaTime;
    float elapsedSeconds;

    public:
        Example();
        void Init();
        void Update();
        void Draw();

    private:
        void DrawMenu(int x, int y, int width);
        void DrawCameraControls(int x, int y, int w);
        void DrawInputInfo(int x, int y);
        void DrawTimingInfo(int x, int y, float scale);
        void DrawImage();
        void DrawVersion();
        void DrawSprites();
        void DrawIcosa();
        void DrawScene(Scene* scene, const vec3& scale);

        bool mouseClick;
        bool musicLooping;

};
