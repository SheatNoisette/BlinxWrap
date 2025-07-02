#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <malloc.h>

#include "pch.h"
#include "blinxwrap_ctx.h"
#include "BinkHandle.h"
#include "debug.h"

#define BINKDLL
#include "BinkFnc.h"

BinkHandle* __stdcall BinkOpen(const char* name, unsigned int flags) {
	BlinxWrapCtx* bctx = NULL;
	BinkHandle* hdl = NULL;

	DebugPrintfA("BLINXWRAP(BinkOpen): Asked to load \"%s\"!", name);

	bctx = malloc(sizeof(BlinxWrapCtx));
	if (bctx == NULL)
		return NULL;

	hdl = binkhandle_from_blinxwrap(bctx);

	// Zanzarah hardcoded defaults
	hdl->frames = 25 * 3;
	hdl->width = 640;
	hdl->height = 480;

	return hdl;
}

void __stdcall BinkClose(BinkHandle* handle) {
	BlinxWrapCtx* bctx = NULL;
	if (handle != NULL)
		return;

	bctx = blinxwrap_from_binkhandle(handle);

	if (bctx != NULL)
		free(bctx);
}

int __stdcall BinkWait(BinkHandle* handle) {
	Sleep((1/25) * 1000);
	return 0;
}

int __stdcall BinkDoFrame(BinkHandle* handle) {
	return 0;
}

void __stdcall BinkNextFrame(BinkHandle* handle) {
}

void __stdcall BinkSetFrameRate(int fps, int flags) {
}

int __stdcall BinkCopyToBuffer(BinkHandle* handle, void* dst, int pitch, unsigned int dheight, unsigned int dx, unsigned int dy, unsigned int flags) {
    char* currentScanline = (char*)dst + (dy * pitch) + (dx * sizeof(DWORD));

    for (unsigned int y = 0; y < dheight; y++) {
        DWORD* rowPixels = (DWORD*)currentScanline;
		// Iterate across the width of the pitch
        for (unsigned int x = 0; x < (pitch / sizeof(DWORD)); x++) { 
            rowPixels[x] = (0xff << 32) | ((x % 255) << 16) | ((x % 255) << 8) | (x % 255);
        }
        // Move to the next scanline
        currentScanline += pitch;
    }

    return 0;
}

int __stdcall BinkDDSurfaceType(void* dd_surface) {
	return 0xA;
}

int __stdcall BinkSetSoundSystem(void* backend, unsigned long param) {
	return 0;
}

void* __stdcall BinkOpenMiles(void* mss_handle) {
	return NULL;
}

void __stdcall BinkSetVolume(BinkHandle* handle, unsigned int id, int volume) {

}