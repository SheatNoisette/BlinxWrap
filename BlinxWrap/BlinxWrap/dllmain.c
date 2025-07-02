#define WIN32_LEAN_AND_MEAN

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>
#include <psapi.h>

#include "av_utils.h"
#include "binary_patching.h"
#include "debug.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DebugPrintfW(L"BLINXWRAP: Loaded !");
        bav_utils_printall_codec();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

