#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <malloc.h>

#include "pch.h"
#include "blinxwrap_ctx.h"
#include "BinkHandle.h"
#include "debug.h"

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/error.h"

#define BINKDLL
#include "BinkFnc.h"

// Zanzarah Enforced
#define GAME_WIDTH 640
#define GAME_HEIGHT 480

BinkHandle* __stdcall BinkOpen(const char *name, unsigned int flags)
{
	BlinxWrapCtx *ctx = NULL;
	BinkHandle *hdl = NULL;

#if BLINX_AGGRESSIVE_DEBUGGING
	DebugPrintfA("Linked headers: avcodec %d  avformat %d\n",
             LIBAVCODEC_VERSION_MAJOR, LIBAVFORMAT_VERSION_MAJOR);
	DebugPrintfA("Running  dlls : %d / %d\n",
				avcodec_version()  >> 16,
				avformat_version() >> 16);
#endif
    LOGA("called with \"%s\" flags=0x%X\n", name ? name : "(null)", flags);

	// Create context
    ctx = calloc(1, sizeof(BlinxWrapCtx));
	hdl = &ctx->bhandle;
    if (!ctx) {
        LOGA("memory allocation failed\n");
        LOG_EXIT(NULL);
    }

	// Check if the video exists
	DWORD dwAttrib = GetFileAttributesA(name);
	if ((dwAttrib == INVALID_FILE_ATTRIBUTES)) {
		LOGA("file not found, returning a fake handle\n");
		// Bypass the init
		ctx->eof = 1;
		hdl->width  = GAME_WIDTH;
		hdl->height = GAME_HEIGHT;
		hdl->frames = 1;
		LOG_EXIT(hdl);
	}

    // Open container
    int ret = avformat_open_input(&ctx->fmt, name, NULL, NULL);
    if (ret < 0) { LOGA("avformat_open_input: %s\n", AV_ERR2STR(ret)); goto fail; }

    ret = avformat_find_stream_info(ctx->fmt, NULL);
    if (ret < 0) { LOGA("avformat_find_stream_info: %s\n", AV_ERR2STR(ret)); goto fail; }

    LOGA("container opened: streams=%u duration=%" PRId64 " us\n",
         ctx->fmt->nb_streams, ctx->fmt->duration);

    // Grab the first video stream
    for (unsigned int i = 0; i < ctx->fmt->nb_streams; ++i) {
        if (ctx->fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            ctx->vst = ctx->fmt->streams[i];
            break;
        }
    }
    if (!ctx->vst) { LOGA("no video stream found\n"); goto fail; }
    LOGA("video stream index=%d\n", ctx->vst->index);

    // Open decoder
    const AVCodec *codec = avcodec_find_decoder(ctx->vst->codecpar->codec_id);
    if (!codec) { LOGA("decoder not found for codec id %d\n",
                       ctx->vst->codecpar->codec_id); goto fail; }

    ctx->dec = avcodec_alloc_context3(codec);
    if (!ctx->dec) { LOGA("avcodec_alloc_context3 failed\n"); goto fail; }

    ret = avcodec_parameters_to_context(ctx->dec, ctx->vst->codecpar);
    if (ret < 0) { LOGA("avcodec_parameters_to_context: %s\n", AV_ERR2STR(ret)); goto fail; }

    ret = avcodec_open2(ctx->dec, codec, NULL);
    if (ret < 0) { LOGA("avcodec_open2: %s\n", AV_ERR2STR(ret)); goto fail; }

    LOGA("decoder opened: %s %dx%d pix_fmt=%s\n",
         codec->name, ctx->dec->width, ctx->dec->height,
         av_get_pix_fmt_name(ctx->dec->pix_fmt));

    // Build frame + scaler
    ctx->raw = av_frame_alloc();
    ctx->rgb = av_frame_alloc();
    if (!ctx->raw || !ctx->rgb) { LOGA("av_frame_alloc failed\n"); goto fail; }

	// Add when we injected the payload to force a HIGHER thing
	int src_w = ctx->dec->width;
	int src_h = ctx->dec->height;

	// Zanzarah hardcode the width and height
	int dst_w = GAME_WIDTH;
	int dst_h = GAME_HEIGHT;

	ret = av_image_alloc(ctx->rgb->data, ctx->rgb->linesize,
						dst_w, dst_h, AV_PIX_FMT_BGRA, 1);
	if (ret < 0) { LOGA("av_image_alloc: %s\n", AV_ERR2STR(ret)); goto fail; }

	// SCALER SRC TO DEST
	ctx->sws = sws_getContext(src_w, src_h, ctx->dec->pix_fmt,
							dst_w, dst_h, AV_PIX_FMT_BGRA,
							SWS_BILINEAR, NULL, NULL, NULL);
	if (!ctx->sws) { LOGA("sws_getContext failed\n"); goto fail; }

	// Extract frames time
    AVRational fr = ctx->vst->avg_frame_rate.num ?
                    ctx->vst->avg_frame_rate :
                    ctx->vst->r_frame_rate;

   	// Do new handle
    hdl->width  = dst_w;
    hdl->height = dst_h;
	// May be zero
    hdl->frames = (int)ctx->vst->nb_frames;
	LOGA("nb_frames field says %i frames\n", hdl->frames);

	// If we got frame == 0 (Wierd codecs), we are faking the number of frames
	if (hdl->frames == 0) {
		LOGA("nb_frames field says 0 frames!\n");

		if (ctx->vst->duration != AV_NOPTS_VALUE && fr.num && fr.den) {
			AVRational tb = ctx->vst->time_base;
			int est = av_rescale_rnd(ctx->vst->duration,
										fr.num * (int)tb.num,
										fr.den * (int)tb.den,
										AV_ROUND_NEAR_INF);
    		ctx->bhandle.frames = (int)est;
   			LOGA("estimated frames from duration: %" PRId64 "\n", est);
		}
		else
		{
			LOGA("could not estimate the number of frames, set to 1!\n");
			hdl->frames = 1;
		}
	}

	// May be dynamic, force 25 fps
    if (!fr.num)
		fr = (AVRational){25,1};

    ctx->frame_time_ms = (int)(1000LL * fr.den / fr.num);
    LOGA("fps = %.2f  -> frame_time_ms=%d\n", fr.num ? (double)fr.num / fr.den : 0.0, ctx->frame_time_ms);

    LOGA("open successful - handle=%p\n", hdl);
    LOG_EXIT(hdl);

fail:
    LOGA("initialisation failed - cleaning up\n");
    LOG_EXIT(NULL);
}


void __stdcall BinkClose(BinkHandle *handle)
{
    if (!handle)
		return;

    BlinxWrapCtx *ctx = blinxwrap_from_binkhandle(handle);

	if (!ctx)
		return;

    if (ctx->rgb) {
        av_freep(&ctx->rgb->data[0]);
        av_frame_free(&ctx->rgb);
    }
    av_frame_free(&ctx->raw);

    sws_freeContext(ctx->sws);
    avcodec_free_context(&ctx->dec);
    avformat_close_input(&ctx->fmt);

    av_free(ctx);
}


int __stdcall BinkWait(BinkHandle *handle)
{
    BlinxWrapCtx *ctx = blinxwrap_from_binkhandle(handle);

    if (!ctx)
		return 1;

    Sleep(ctx->frame_time_ms);
    return 0;
}

int __stdcall BinkDoFrame(BinkHandle *handle)
{
#if BLINX_ADDITIONNAL_DEBUGGING
    LOGA("called with handle %p\n", handle);
#endif

    if (!handle)
		LOG_EXIT(1);

    BlinxWrapCtx *ctx = blinxwrap_from_binkhandle(handle);
    if (!ctx)
		LOG_EXIT(2);

	// We done ?
    if (ctx->eof) {
        LOGA("EOF reached earlier - nothing decoded\n");
        LOG_EXIT(0);
    }

    // Prepare an empty packet
    AVPacket pkt = {0};
    int got_frame = 0;

    // Try to output ONE FRAMME
    while (!got_frame) {

        // Attempt to pull a frame that is already buffered
        int ret = avcodec_receive_frame(ctx->dec, ctx->raw);
        if (ret == 0) {
#if BLINX_ADDITIONNAL_DEBUGGING
            LOGA("decoded frame PTS=%" PRId64 "\n", ctx->raw->pts);
#endif

            sws_scale(ctx->sws,
                      (const uint8_t * const*)ctx->raw->data,
                      ctx->raw->linesize,
                      0,
                      ctx->dec->height,
                      ctx->rgb->data,
                      ctx->rgb->linesize);

            ctx->next_pts = ctx->raw->best_effort_timestamp;
            av_frame_unref(ctx->raw);
            got_frame = 1;
            continue;
        }

        if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            LOGA("avcodec_receive_frame() failed: %s\n", AV_ERR2STR(ret));
            LOG_EXIT(3);
        }

        // Need to send more packets to the decoder
        ret = av_read_frame(ctx->fmt, &pkt);
        if (ret < 0) {
			// Container finished
            LOGA("av_read_frame() hit EOF (%s) - flushing decoder\n",
                 (ret == AVERROR_EOF) ? "AVERROR_EOF" : AV_ERR2STR(ret));

			// Flush
            avcodec_send_packet(ctx->dec, NULL);
            ctx->eof = 1;
			// Loop back, try receive
            continue;
        }

#if BLINX_ADDITIONNAL_DEBUGGING
        LOGA("read packet: dts=%" PRId64 " stream=%d size=%d\n",
             pkt.dts, pkt.stream_index, pkt.size);
#endif

        if (pkt.stream_index == ctx->vst->index) {
            ret = avcodec_send_packet(ctx->dec, &pkt);
            if (ret < 0) {
                LOGA("avcodec_send_packet() failed: %s\n", AV_ERR2STR(ret));
                av_packet_unref(&pkt);
                LOG_EXIT(4);
            }
#if BLINX_ADDITIONNAL_DEBUGGING
            LOGA("packet sent to decoder\n");
#endif
        }
        // Discard packet memory, happy decoding !
        av_packet_unref(&pkt);
    }

#if BLINX_ADDITIONNAL_DEBUGGING
    LOG_EXIT(0);
#else
	return 0;
#endif
}


void __stdcall BinkNextFrame(BinkHandle* handle) {
	// NOP
}

void __stdcall BinkSetFrameRate(int fps, int flags) {
	LOGA("BinkSetFrameRate with fps=%i flags=0x%x\n", fps, flags);
}

int __stdcall BinkCopyToBuffer(BinkHandle* handle, void* dst, int pitch, unsigned int dheight, unsigned int dx, unsigned int dy, unsigned int flags) {
    BlinxWrapCtx *ctx = blinxwrap_from_binkhandle(handle);

    if (!ctx || !ctx->rgb || !ctx->rgb->data[0])
		return 1;

    const uint8_t *src = ctx->rgb->data[0];
    int src_pitch      = ctx->rgb->linesize[0];

    uint8_t *out = (uint8_t*)dst + dy*pitch + dx*4; // /!\ 4 bytes per pixel
    unsigned int copy_h = FFMIN(dheight, (unsigned int)ctx->dec->height);

    for (unsigned int y = 0; y < copy_h; y++) {
		memcpy(out + y*pitch, src + y*src_pitch, GAME_WIDTH * 4);
    }

    return 0;
}


int __stdcall BinkDDSurfaceType(void* dd_surface) {
	// From debugger, may be an enum
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