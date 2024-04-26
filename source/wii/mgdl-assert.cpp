#ifndef _MGDL_DEBUG

#include "mgdl-wii/mgdl-assert.h"
#include "mgdl-wii.h"
#include <wiiuse/wpad.h>


void gdl::AssertFunction(const char* message, int lineNumber, const char* filename, bool& ignoreToggle)
{
    static gdl::Font debugFont;
    gdl::WiiInput input = WiiInput();
    static bool fontLoaded = debugFont.LoadFontMem(gdl::DefaultFontData);
    if (false)
    {
        while (true)
        {
            input.StartFrame();
            gdl::PrepDisplay();
            debugFont.Printf(20, 20, 2, 0xFFFFFFFF, "Assert failed! Location: %s line: %d. Message: %s", filename, lineNumber, message);
            debugFont.Printf(20, 60, 2, 0xFFFFFFFF, "Press A to ignore always. Press HOME to Exit");

            bool ignorePressed = input.ButtonPress(WIIMOTE_BUTTON_A);
            if (ignorePressed)
            {
                ignoreToggle = true;
                break;
            }
            bool exitPressed = input.ButtonPress(WIIMOTE_BUTTON_HOME);
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
