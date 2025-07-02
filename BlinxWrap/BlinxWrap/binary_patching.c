#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>
#include <psapi.h>

#include "debug.h"

/*
    ===========================================
    Binary patching from memory utils functions
    ===========================================

    Usage example in a DLL_PROCESS_ATTACH:

    ```c
    BYTE rwBytes[] = { 0xb8, 0x10, 0x03, 0x00, 0x00 };
    BYTE rwBytesTarget[] = { 0xb8, 0x20, 0x04, 0x00, 0x00 };
    // Get base address of the main executable
    HMODULE hExeBase = GetModuleHandleW(NULL);
    DebugPrintfW(L"BLINXWRAP: Found ZanZarah base address: 0x%p", hExeBase);

    DebugPrintfW(L"BLINXWRAP: Attempting to patch RenderWare version...");
    DWORD_PTR target_rva = RVA_GET_RELATIVE_OFFSET(0x4ef910);
    if (MatchSequence(hExeBase, target_rva, rwBytes, 5) == TRUE) {
        DebugPrintfW(L"BLINXWRAP: Found RenderWare version ret! Patching...");
        if (PatchSequence(hExeBase, target_rva, rwBytesTarget, 5) == TRUE) {
           DebugPrintfW(L"BLINXWRAP: RenderWare Version patched!"); 
        }
    }
    ```
*/

// Check if an opcode at a specific address exists and is valid to avoid wrong patching
BOOL MatchSequence(HMODULE base_address, DWORD_PTR offset, const BYTE *opcode, DWORD len) {
    LPVOID target_address = (LPVOID)((DWORD_PTR)base_address + offset);

#if BLINX_AGGRESSIVE_DEBUGGING
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQueryEx(GetCurrentProcess(), target_address, &mbi, sizeof(mbi))) {
        DebugPrintfW(L"BLINXWRAP(MatchSequence): Memory region at 0x%p. Base: 0x%p, Size: 0x%zx, Protect: 0x%lx, State: 0x%lx, Type: 0x%lx",
            target_address, mbi.BaseAddress, mbi.RegionSize, mbi.Protect, mbi.State, mbi.Type);
        if (!(mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READONLY | PAGE_READWRITE))) {
            DebugPrintfW(L"BLINXWRAP(MatchSequence): Warning: Target memory region does not have read permissions!");
        }
        if (mbi.State != MEM_COMMIT) {
            DebugPrintfW(L"BLINXWRAP(MatchSequence): Warning: Target memory region is not committed (State: 0x%lx)!", mbi.State);
        }
    } else {
        DebugPrintfW(L"BLINXWRAP(MatchSequence): VirtualQueryEx failed for 0x%p. Error: %lu", target_address, GetLastError());
    }
#endif

    BYTE *read_buffer = (BYTE *)malloc(len);
    if (!read_buffer) {
        DebugPrintfW(L"BLINXWRAP(MatchSequence): Failed to allocate memory for read_buffer.");
        return FALSE;
    }

    SIZE_T bytesRead;
    BOOL result = FALSE;

#if BLINX_AGGRESSIVE_DEBUGGING
    DebugPrintfW(L"BLINXWRAP(MatchSequence) : Target address 0x%p to verify, reading...", target_address);
#endif

    if (ReadProcessMemory(GetCurrentProcess(), target_address, read_buffer, len, &bytesRead)) {
        if (bytesRead == len) {
            if (memcmp(opcode, read_buffer, len) == 0) {
                result = TRUE;
            } else {
#if BLINX_AGGRESSIVE_DEBUGGING
                DebugPrintfW(L"BLINXWRAP(MatchSequence): Mismatch at 0x%p. Expected:", target_address);
                for (DWORD i = 0; i < len; i++) DebugPrintfW(L"%02X ", opcode[i]);
                DebugPrintfW(L"\nBLINXWRAP(MatchSequence): Actual:");
                for (DWORD i = 0; i < len; i++) DebugPrintfW(L"%02X ", read_buffer[i]);
                DebugPrintfW(L"\n");
#endif
            }
        } else {
            DebugPrintfW(L"BLINXWRAP(MatchSequence): ReadProcessMemory read %zu bytes, expected %lu bytes at 0x%p.", bytesRead, len, target_address);
        }
    } else {
        DWORD lastError = GetLastError();
        DebugPrintfW(L"BLINXWRAP(MatchSequence): ReadProcessMemory failed at 0x%p. Error: %lu", target_address, lastError);
    }

    free(read_buffer); // Free the allocated buffer
    return result;
}

BOOL PatchSequence(HMODULE base_address, DWORD_PTR offset_rva, const BYTE *data, DWORD len) {
    LPVOID target_address = (LPVOID)((DWORD_PTR)base_address + offset_rva);
    BOOL result = FALSE;
    DWORD oldProtect = 0;

#if BLINX_AGGRESSIVE_DEBUGGING
    DebugPrintfW(L"BLINXWRAP(PatchSequence): Attempting to patch at 0x%p (RVA 0x%x).", target_address, offset_rva);
    DebugPrintfW(L"BLINXWRAP(PatchSequence): Data to write:");
    for (DWORD i = 0; i < len; i++) {
        WCHAR temp[5];
        swprintf_s(temp, sizeof(temp) / sizeof(WCHAR), L"%02X ", data[i]);
        OutputDebugStringW(temp);
    }
    OutputDebugStringW(L"\n");
#endif

    // Change memory protection to allow writing
    // We need PAGE_EXECUTE_READWRITE or PAGE_READWRITE to write to the memory.
    // PAGE_EXECUTE_READWRITE is often preferred for code sections.
    if (!VirtualProtectEx(GetCurrentProcess(), target_address, len, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        DWORD lastError = GetLastError();
        DebugPrintfW(L"BLINXWRAP(PatchSequence): VirtualProtectEx failed to change protection at 0x%p. Error: %lu", target_address, lastError);
        return FALSE;
    }

    // Write the new byte sequence
    SIZE_T bytesWritten;
    if (WriteProcessMemory(GetCurrentProcess(), target_address, data, len, &bytesWritten)) {
        if (bytesWritten == len) {
            result = TRUE;
            DebugPrintfW(L"BLINXWRAP(PatchSequence): Successfully patched %lu bytes at 0x%p.", len, target_address);
        } else {
            DebugPrintfW(L"BLINXWRAP(PatchSequence): WriteProcessMemory wrote %zu bytes, expected %lu bytes at 0x%p.", bytesWritten, len, target_address);
        }
    } else {
        DWORD lastError = GetLastError();
        DebugPrintfW(L"BLINXWRAP(PatchSequence): WriteProcessMemory failed at 0x%p. Error: %lu", target_address, lastError);
    }

    // Restore original memory protection
    DWORD tempOldProtect;
    if (!VirtualProtectEx(GetCurrentProcess(), target_address, len, oldProtect, &tempOldProtect)) {
        DebugPrintfW(L"BLINXWRAP(PatchSequence): Failed to restore original protection at 0x%p. Error: %lu", target_address, GetLastError());
    }

    if (!FlushInstructionCache(GetCurrentProcess(), target_address, len)) {
        DebugPrintfW(L"BLINXWRAP(PatchSequence): Failed to flush instruction cache at 0x%p. Error: %lu", target_address, GetLastError());
    }

    return result;
}
