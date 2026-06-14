#pragma once

/**
* @file mgdl-console.h
* @ingroup core
* @brief Functions to use the Wii/Linux/Win/etc console
*/

/**
 * @brief Enums for colors available in the console
 */
enum ConsoleTextColor
{
    ConsoleText_Black = 30,
    ConsoleText_Red,
    ConsoleText_Green,
    ConsoleText_Brown,
    ConsoleText_Blue,
    ConsoleText_Magenta,
    ConsoleText_Cyan,
    ConsoleText_White,
    ConsoleText_Default = 39
};
typedef enum ConsoleTextColor ConsoleTextColor;

// https://www.man7.org/linux/man-pages/man4/console_codes.4.html

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Print colorful text to console
* @param color What color to use
* @param text Text to print
*/
void Console_Print(ConsoleTextColor color, const char* text);

/**
* @brief Print colorful formatted text to console
* @param color What color to use
* @param text Formatting string
* @param VA_ARGS Parameters to format parameters
*/
void Console_Printf(ConsoleTextColor color, const char* format, ...);

void Console_SetTextColor(ConsoleTextColor color);
void Console_ResetTextColor();

#define MGDL_CONSOLECOLOR_PRE "\033["
#define MGDL_CONSOLECOLOR_POST "m"
#define MGDL_CONSOLECOLOR_RESET "\033[0m"

#ifdef __cplusplus
}
#endif
