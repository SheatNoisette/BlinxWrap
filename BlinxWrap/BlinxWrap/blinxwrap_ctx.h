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
    /* FFmpeg handles */
    AVFormatContext *fmt;
    AVCodecContext  *dec;
    AVStream        *vst;
    struct SwsContext *sws;      // YUV -> BGRA converter
    AVFrame  *raw;               // decoded frame (codec native fmt)
    AVFrame  *rgb;               // converted BGRA frame
    int64_t   next_pts;          // PTS of frame that BinkDoFrame will output next
    int       eof;               // set when av_read_frame() reaches EOF
    int       frame_time_ms;     // 1000 / fps ; used by BinkWait()

    // Public Bink facade
    BinkHandle bhandle;
} BlinxWrapCtx;

BlinxWrapCtx *blinxwrap_from_binkhandle(BinkHandle *bh);
BinkHandle *binkhandle_from_blinxwrap(BlinxWrapCtx *bwctx);

#endif