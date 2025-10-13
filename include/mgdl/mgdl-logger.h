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
	All = 1,
	Info = 1,
	Warning = 2,
	Error = 3
};
typedef enum LogLevel LogLevel;

#ifdef __cplusplus
extern "C"
{
#endif
	void Log_SetLevel(LogLevel lvl);

	void Log_Info(const char* text);
	void Log_InfoF(const char* fmt, ...);
	void Log_Warning(const char* text);
	void Log_WarningF(const char* fmt, ...);
	void Log_Error(const char* text);
	void Log_ErrorF(const char* fmt, ...);

	void _Log_Print(const char* text);

#ifdef __cplusplus
}
#endif
