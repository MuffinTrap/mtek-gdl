#pragma once

#include <mgdl.h>

class Example
{
    gdl::Image* barb;
    gdl::SpriteSet mel_sprites;
    gdl::Image* pointerImage;
    gdl::Font* ibmFont;
    gdl::Font* debugFont;
    gdl::Sound* sampleMusic;
    gdl::Sound* blip;
    gdl::MenuCreator menu;
    gdl::MenuCreator cameraMenu;
    gdl::Scene* wiiScene;
    gdl::Image* wiiTexture;

    gdl::Image* matcapTexture;
    gdl::Scene* icosaScene;
    gdl::Material* matcapMaterial;

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
        void DrawScene(gdl::Scene* scene, const vec3& scale);

        bool mouseClick;
        bool musicLooping;

};
