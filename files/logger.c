#include "logger.h"
#include "assembler.h"

#include <stdio.h>
#include <stdarg.h>

/* colors */
#define WHITE_COLOR "\x1B[37m"
#define ERROR_COLOR "\x1B[31m"
#define SUCCESS_COLOR "\x1B[32m"

 /**
 * function for error output
 * @param format string that contains the text to be written to stdout
 * @param ... The arguments to format into the text
 */ 
void log_error(const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	
	if (!current_line)
		printf("%s[-] %serror >> %s\n", WHITE_COLOR, ERROR_COLOR, buffer);
	else
		printf("%s[-] %serror on line %d >> %s\n", WHITE_COLOR, ERROR_COLOR, current_line, buffer);
}

 /**
 * function for success output
 * @param format string that contains the text to be written to stdout
 * @param ... The arguments to format into the text
 */ 
void log_success(const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	
	printf("%s[+] %s%s\n", WHITE_COLOR, SUCCESS_COLOR, buffer);
}

 /**
 * function for information output
 * @param format string that contains the text to be written to stdout
 * @param ... The arguments to format into the text
 */ 
void log_info(const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	
	printf("%s[!] %s\n", WHITE_COLOR, buffer);
}
