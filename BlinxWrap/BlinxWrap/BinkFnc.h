#ifndef __BINK_FNC_H__
#define __BINK_FNC_H__

#include "BinkHandle.h"

#ifdef BINKDLL
#define BINKEXPORT __declspec(dllexport)
#else
#define BINKEXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

	BINKEXPORT BinkHandle* __stdcall BinkOpen(const char* name, unsigned int flags);
	BINKEXPORT void __stdcall BinkClose(BinkHandle* handle);

	BINKEXPORT int __stdcall BinkWait(BinkHandle* handle);
	BINKEXPORT int __stdcall BinkDoFrame(BinkHandle* handle);
	BINKEXPORT void __stdcall BinkNextFrame(BinkHandle* handle);
	BINKEXPORT void __stdcall BinkSetFrameRate(int fps, int flags);

	BINKEXPORT int __stdcall BinkCopyToBuffer(BinkHandle* handle, void* dst, int pitch, unsigned int dheight, unsigned int dx, unsigned int dy, unsigned int flags);
	BINKEXPORT int __stdcall BinkDDSurfaceType(void* dd_surface);

	BINKEXPORT int __stdcall BinkSetSoundSystem(void* backend, unsigned long param);
	BINKEXPORT void* __stdcall BinkOpenMiles(void* mss_handle);
	BINKEXPORT void __stdcall BinkSetVolume(BinkHandle* handle, unsigned int id, int volume);

#ifdef __cplusplus
	// extern "C"
}
#endif

#endif