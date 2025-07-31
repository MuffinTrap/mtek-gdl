#include <mgdl/mgdl-console.h>
#include <mgdl/mgdl-util.h>
#include <stdio.h>

void Console_Print(ConsoleTextColor color, const char* text)
{
    printf("\033[%dm%s\033[0m", (int)color, text);
}

void Console_Printf(ConsoleTextColor color, const char* format, ...)
{
	MGDL_PRINTF_TO_BUFFER(format);

    Console_Print(color, mgdl_GetPrintfBuffer());
}
