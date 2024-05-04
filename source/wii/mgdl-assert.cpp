// muffintrap: this function is used by the library
// and you can use it too!
#include <mgdl-wii.h>
#include <wiiuse/wpad.h>
#include <stdio.h>
#include <stdarg.h>
void AssertFunction(const char* filename, int lineNumber, bool& ignoreToggle, const char* message, ...)
{
    gdl::ConsoleMode();
        char	buff[256];
        va_list args;

        va_start(args, message);
        vsprintf(buff, message, args);
        va_end(args);

    printf("Assert failed! %s:%d:%s\n", filename, lineNumber, buff);
    printf("Press A to ignore, HOME to quit\n");
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
            break;
        }
        VIDEO_WaitVSync();
    }

    if (ignoreToggle == false)
    {
        VIDEO_SetBlack(TRUE);
        VIDEO_Flush();
        VIDEO_WaitVSync();
        VIDEO_WaitVSync();
        exit(0);
    }
}
