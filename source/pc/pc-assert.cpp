#include <mgdl/mgdl-assert.h>

#include <stdio.h>
#include <stdarg.h>
#include <cassert>
void AssertFunctionPrintf(const char* filename, int lineNumber, const char* message, ...)
{
	char	buff[256];
	va_list args;

	va_start(args, message);
	vsnprintf(buff, 256, message, args);
	va_end(args);

	printf("Assert failed! %s:%d:%s\n", filename, lineNumber, buff);
	printf("Press enter to proceed to assert handler\n");
	char answer = '\0';
	scanf("%c", &answer);
	assert(false);
}

void AssertFunctionPrint(const char* filename, int lineNumber, const char* message)
{
	printf("Assert failed! %s:%d:%s\n", filename, lineNumber, message);
	printf("Press enter to proceed to assert handler\n");
	char answer = '\0';
	scanf("%c", &answer);
	assert(false);
}

