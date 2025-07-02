#ifndef __DEBUG_BLINX__
#define __DEBUG_BLINX__

#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>

#define BLINX_AGGRESSIVE_DEBUGGING 1

void DebugPrintf(const WCHAR* format, ...);

#endif