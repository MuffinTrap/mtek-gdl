#pragma once

#include <mgdl-wii.h>

class Template
{
    gdl::Image barb;
    gdl::Image mel_image;
    gdl::SpriteSet mel_sprites;
    gdl::Image ibmFontImage;
    gdl::Image pointerImage;
    gdl::FFont ibmFont;
    gdl::Music sampleMusic;
    gdl::Sound blip;
    gdl::MenuCreator menu;

    public:
        Template();
        void Init();
        void Update();
        void Draw();

    private:
        void DrawMenu(int x, int y, int width);
        void DrawInputInfo(int x, int y);
        void DrawTimingInfo(int x, int y, float scale);
        void DrawSprites();
};