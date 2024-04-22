#pragma once

#include "mgdl-wii.h"

namespace gdl
{
    class MenuCreator
    {

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
        void Panel(int x, int y, int w, int h, int col, short style);
        void Text(int x, int y, const char* text);
        bool Button(int x, int y, int w, int h, const char* text);
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
        gdl::Font& currentFont;
};
*/
