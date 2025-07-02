#ifndef __DEBUG_BLINX__
#define __DEBUG_BLINX__

#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>

#define BLINX_AGGRESSIVE_DEBUGGING 1

void DebugPrintfW(const WCHAR* format, ...);
void DebugPrintfA(const CHAR* format, ...);
void charToWChar(const char* char_str, wchar_t* wchar_buf, size_t buf_size);

#endif