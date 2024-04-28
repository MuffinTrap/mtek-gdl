#pragma once

#include "mgdl-wii.h"
#include "mgdl-wii/mgdl-input-wii.h"

namespace gdl
{
    class MenuCreator
    {
        public :
        MenuCreator(gdl::FFont font, short fontHeight);
        void StartMenu(int x, int y, int w, int h, float fontScale);
        void Panel(int h, u_int col, short style);
        void Text(const char* text);
        bool Button(const char* text, u_int color);

        private:
        int x;
        int y;
        int w;
        int h;

        gdl::FFont currentFont;
        short fontHeight;
        float fontScale;
        private:
        gdl::WiiInput *input;
    };
};

/* Work in progress. Completely changed the original file 
    TODO: 
        - Add gui state struct
        - Add menu state struct
            - Add layout variables
*/

/*

namespace gdl {

enum PanelStyle{
    OutPanel = 0,
    InPanel  = 1,
    TextBox  = 2,
    Slider   = 3,
    InFrame  = 4,
};

typedef struct {
    short keyChar;
    int keyCode;
    bool keyHit;
} KeyboardInputStruct;

typedef struct {
    int x;
    int y;
    float Hscroll;
    float Vscroll;
    bool Left;
    bool Right;
    bool Middle;
    bool ClickLeft;
    bool ClickRight;
    bool ClickMiddle;
} MouseInputStruct;

class MenuCreator {

    public:
        MenuCreator();

        // Basic controls
        bool CheckBox(int x = 0, int y = 0, const char* caption = NULL, bool& value, bool disabled = false);
        float Slider(int x, int y, int w, int h, float min, float max, float step, float& value);

        void StartListBox(int x = 0, int y = 0, int w = 0, int h = 0, int selected);
        void ListBoxItem(const char* text);
        int EndListBox(); // Return selected index

        // Textbox status? Writing, Complete, Empty
        bool TextBox(int x = 0, int y = 0, int w, int h, char* textBuff = NULL, int textBuffSize = 0, bool disabled = false);

        void SetFont(gdl::Font& font);

        // Maybe later, these are really hard to do
        float HScrollBar(int minVal, int maxVal, int x, int y, int w, int h, int step = 1);
        float VScrollBar(int minVal, int maxVal, int x, int y, int w, int h, int step = 1);

    private:
        bool CursorInsideRect(int x, int y, int w, int h);
        // One more for Wii input
        MouseInputStruct mouseInput;
        KeyboardInputStruct keyboardInput;
};
*/
