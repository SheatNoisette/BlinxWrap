#include "pch.h"

#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

#include "blinxwrap_ctx.h"
#include "BinkHandle.h"

// Extract BlinxWrap context from a Binkhandle
BlinxWrapCtx *blinxwrap_from_binkhandle(BinkHandle *bh)
{
    return (BlinxWrapCtx *)(
        (uint8_t *)bh - offsetof(BlinxWrapCtx, bhandle)
    );
}

// Extract Binkhandle from a Binkhandle context
BinkHandle *binkhandle_from_blinxwrap(BlinxWrapCtx *bwctx)
{
    return &bwctx->bhandle;
}