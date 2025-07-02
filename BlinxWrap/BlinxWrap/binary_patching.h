#ifndef __BINARY_PATCHING_H__
#define __BINARY_PATCHING_H__

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>
#include <psapi.h>

#include "debug.h"

// Compute offset from a fixed 32bit offset
#define RVA_GET_RELATIVE_OFFSET(addr) ((addr) - 0x400000)

BOOL MatchSequence(HMODULE base_address, DWORD_PTR offset, const BYTE *opcode, DWORD len);
BOOL PatchSequence(HMODULE base_address, DWORD_PTR offset_rva, const BYTE *data, DWORD len);

#endif