#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>

// Simple DebugPrintF when the debugger is not available
void DebugPrintf(const WCHAR* format, ...) {
    WCHAR buffer[1024];
    va_list args;
    va_start(args, format);
    vswprintf_s(buffer, sizeof(buffer) / sizeof(WCHAR), format, args);
    va_end(args);
    OutputDebugStringW(buffer);
}
