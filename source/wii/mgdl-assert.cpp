#ifndef _MGDL_DEBUG

#include "mgdl-wii/mgdl-assert.h"
#include "mgdl-wii.h"
#include <wiiuse/wpad.h>


void gdl::AssertFunction(const char* message, int lineNumber, const char* filename, bool& ignoreToggle)
{
    if (false)
    {
        gdl::ConsoleMode();
        printf("Assert failed! Location: %s line: %d. Message: %s\n", filename, lineNumber, message);
        printf("Press A to ignore, HOME to quit.\n");
        while (true)
        {
            WPAD_ScanPads();
            if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A)
            {
                ignoreToggle = true;
                break;
            }
            if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
            {
                // Return to homebrew menu
                gdl::wii::Exit();
                break;
            }
        }
    }
}
#endif // _MGDL_DEBUG
