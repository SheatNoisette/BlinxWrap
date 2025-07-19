#ifndef __DEBUG_BLINX__
#define __DEBUG_BLINX__

#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>
#include "libavutil/error.h"

#define BLINX_AGGRESSIVE_DEBUGGING 1
#define BLINX_ADDITIONNAL_DEBUGGING 0

#define AV_ERR2STR(e) av_err2str((e))
#define LOGA(...) DebugPrintfA("BLINXWRAP: " __VA_ARGS__)
#define LOG_EXIT(rc) do { LOGA("-> return %d\n", (rc)); return (rc); } while (0)

void DebugPrintfW(const WCHAR* format, ...);
void DebugPrintfA(const CHAR* format, ...);
void charToWChar(const char* char_str, wchar_t* wchar_buf, size_t buf_size);

#endif