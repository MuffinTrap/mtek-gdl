#pragma once

/**
 * @file mgdl-logger.h
 * @ingroup core
 * @brief Functions for printing log messages.
 *
 */

enum LogLevel
{
	None = 0,
	Info = 1,
	Warning = 2,
	Error = 3,
	All = 4
};
typedef enum LogLevel LogLevel;

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief Sets the log level. Controls what messages are printed on screen.
	 * @param lvl Level of the log.
	 */
	void Log_SetLevel(LogLevel lvl);
	/**
	 * @brief Instructs to save a set amount of latesta messages.
	 * @param amount How many messages to save
	 */
	void Log_SaveLines(int amount);
	/**
	 * @brief Gets a saved line indexed from oldest to latest.
	 * @param index The index of the line
	 * @returns Pointer to the message or nullptr if no message or invalid index.
	 * @note Only works if Log_SaveLines has been called previously.
	 */
	char* Log_GetLine(int index);
	/**
	 * @brief Gets a saved line indexed from latest to oldest.
	 * @param index The index of the line
	 * @returns Pointer to the message or nullptr if no message or invalid index.
	 * @note Only works if Log_SaveLines has been called previously.
	 */
	char* Log_GetLastLine(int index);
	/**
	 * @brief Sets the index where the next message is saved.
	 * @param index The index to use for next message
	 */
	void Log_SetLineIndexTo(int index);
	/**
	 * @brief Gets the current line index.
	 * @returns The index where the next message will be saved
	 */
	int Log_GetLineIndex();

	/**
	 * @brief Log information. Use for status changes, file properties and such
	 * @param text The message
	 */
	void Log_Info(const char* text);
	void Log_InfoF(const char* fmt, ...);
	/**
	 * @brief Log a warning. Use when something is strange or special.
	 * @param text The message
	 */
	void Log_Warning(const char* text);
	void Log_WarningF(const char* fmt, ...);
	/**
	 * @brief Log an error. Use when something breaks or is wrong or missing.
	 * @param text The message
	 */
	void Log_Error(const char* text);
	void Log_ErrorF(const char* fmt, ...);


#ifdef __cplusplus
}
#endif
