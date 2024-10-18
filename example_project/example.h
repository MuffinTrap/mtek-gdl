#pragma once

#include <mgdl.h>

class Example
{
    gdl::Image* barb;
    gdl::Image* mel_image;
    gdl::SpriteSet mel_sprites;
    gdl::Image* ibmFontImage;
    gdl::Image* pointerImage;
    gdl::Font* ibmFont;
    gdl::Sound* sampleMusic;
    gdl::Sound* blip;
    gdl::MenuCreator menu;
    gdl::Scene* wiiScene;
    gdl::Image* wiiTexture;

    float deltaTime;
    float elapsedSeconds;

    public:
        Example();
        void Init();
        void Update();
        void Draw();

    private:
        void DrawMenu(int x, int y, int width);
        void DrawInputInfo(int x, int y);
        void DrawTimingInfo(int x, int y, float scale);
        void DrawSprites();

};
