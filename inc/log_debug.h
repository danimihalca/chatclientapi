#ifndef LOG_DEBUG_H
#define LOG_DEBUG_H

#include <cstdarg>
#include <cstdio>

#ifdef ANDROID
 #include <android/log.h>
#endif

#ifdef _MSC_VER
    #define FUNCTION_NAME __FUNCTION__
#else
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#define log_debug(...) LogDebug(FUNCTION_NAME, __VA_ARGS__)

inline void LogDebug(const char* tag, const char* format, ...)
{
#ifdef LOG_DEBUG
    va_list arguments;
    va_start(arguments,format);

#if defined(ANDROID)
    __android_log_vprint(ANDROID_LOG_DEBUG,tag,format,arguments);
#elif defined(WINDOWS_PHONE)
    wchar_t buffer[100];
    _vsnwprintf_s(buffer, BUFFER_LENGTH, _TRUNCATE, format, arguments);
    buffer[99] = '\0';
    OutputDebugString(buffer);
#else
    char buffer[100];
    strcpy(buffer,"D/");
    strcat(buffer,tag);
    strcat(buffer,": ");
    strcat(buffer,format);
    vprintf(buffer,arguments);
#endif
    va_end(arguments);
#endif // defined DEBUG
}
#endif // LOG_DEBUG_H

