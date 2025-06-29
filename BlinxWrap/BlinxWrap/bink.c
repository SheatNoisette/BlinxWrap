#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <malloc.h>

#include "pch.h"
#include "BinkHandle.h"

#define BINKDLL
#include "BinkFnc.h"

BinkHandle* __stdcall BinkOpen(const char* name, unsigned int flags) {
	BinkHandle* hdl = malloc(sizeof(BinkHandle));

	if (hdl == NULL)
		return NULL;

	hdl->frames = 1;
	// Zanzarah hardcoded defaults
	hdl->width = 640;
	hdl->height = 480;

	return hdl;
}

void __stdcall BinkClose(BinkHandle* handle) {
	if (handle != NULL)
		free(handle);
}

int __stdcall BinkWait(BinkHandle* handle) {
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