#ifndef __BLINX_WRAP_CTX_H__
#define __BLINX_WRAP_CTX_H__

#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

#include "BinkHandle.h"

// BlixWrapContext: Hold the current context and data of the wrapper, hidden
// from the target application
typedef struct BlinxWrapCtx {
    AVFormatContext *fmt_ctx;

    struct BinkHandle bhandle;
} BlinxWrapCtx;

BlinxWrapCtx *blinxwrap_from_binkhandle(BinkHandle *bh);
BinkHandle *binkhandle_from_blinxwrap(BlinxWrapCtx *bwctx);

#endif