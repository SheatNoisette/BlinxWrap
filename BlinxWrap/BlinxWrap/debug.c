#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>

// Simple DebugPrintF when the debugger is not available
void DebugPrintfW(const WCHAR* format, ...) {
    WCHAR buffer[1024];
    va_list args;
    va_start(args, format);
    vswprintf_s(buffer, sizeof(buffer) / sizeof(WCHAR), format, args);
    va_end(args);
    OutputDebugStringW(buffer);
}

// Simple DebugPrintF when the debugger is not available
void DebugPrintfA(const CHAR* format, ...) {
    CHAR buffer[1024];
    va_list args;
    va_start(args, format);
    vsprintf_s(buffer, sizeof(buffer) / sizeof(CHAR), format, args);
    va_end(args);
    OutputDebugStringA(buffer);
}

// Helper function to convert char* to wchar_t*
void charToWChar(const char* char_str, wchar_t* wchar_buf, size_t buf_size) {
    if (char_str) {
        mbstowcs_s(NULL, wchar_buf, buf_size, char_str, _TRUNCATE);
    } else {
        wcscpy_s(wchar_buf, buf_size, L"N/A");
    }
}