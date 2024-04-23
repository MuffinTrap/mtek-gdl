#ifdef _MGDL_DEBUG

#include "mgdl-wii/mgdl-assert.h"
#include "mgdl-wii/mgdl-global-internals.h"
#include "mgdl-wii/mgdl-main.h"
#include "mgdl-wii/mgdl-input-wii.h"
#include "mgdl-wii/mgdl-font.h"
#include "mgdl-wii/mgdl-defaultfont.h"
#include <wiiuse/wpad.h>


void gdl::AssertFunction(bool test, char* message, int lineNumber, char* filename, bool& ignoreToggle)
{
    static gdl::Font debugFont;
    static bool fontLoaded = debugFont.LoadFontMem(gdl::DefaultFontData);
    if (fontLoaded)
    {
        while (true)
        {
            gdl::WiiInput::StartFrame();
            gdl::PrepDisplay();
            debugFont.printf(20, 20, 2, 0xFFFFFFFF, "Assert failed! Location: %s line: %d. Message: %s", filename, lineNumber, message);
            debugFont.printf(20, 60, 2, 0xFFFFFFFF, "Press A to ignore always. Press HOME to Exit");

            bool ignorePressed = gdl::WiiInput::ButtonPress(WIIMOTE_BUTTON_A);
            if (ignorePressed)
            {
                ignoreToggle = true;
                break;
            }
            bool exitPressed = gdl::wiiInput::ButtonPress(WIIMOTE_BUTTON_HOME);
            if (exitPressed) 
            {
                // Return to homebrew menu
                gdl::wii::Exit();
            }
            gdl::Display();
        }
    }
    else 
    {
        // Failed to load default font
        gdl::ConsoleMode();
        printf("Assert failed! Location: %s line: %d. Message: %s\n", filename, lineNumber, message);
    }
}
#endif // _MGDL_DEBUG
