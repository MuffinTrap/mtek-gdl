#include <mgdl-pc/mgdl-pc-assert.h>

#include <stdio.h>
#include <stdarg.h>
void AssertFunctionPrintf(bool test, const char* filename, int lineNumber, const char* message, ...)
{
	if (test == false)
	{
		char	buff[256];
		va_list args;

		va_start(args, message);
		vsnprintf(buff, 256, message, args);
		va_end(args);

		printf("Assert failed! %s:%d:%s\n", filename, lineNumber, buff);
		assert(test);
	}
}
void AssertFunctionPrint(bool test, const char* filename, int lineNumber, const char* message)
{
	if (test == false)
	{
		printf("Assert failed! %s:%d:%s\n", filename, lineNumber, message);
		assert(test);
	}
}

