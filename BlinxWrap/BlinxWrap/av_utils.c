#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <corecrt_wstdio.h>

#include "libavcodec/codec.h"
#include "libavutil/log.h"

#include "debug.h"
#include "av_utils.h"

// Print codec capability as it to see what codec are supported in this build
// of avcodec/ffmpeg. Depending of the prebuilt version, this may change, ugh...
void bav_utils_printall_codec() {
    void *i = NULL;
    const AVCodec *codec;
    char capabilities[AV_PRINT_NAME_BUFFER] = { 0 };
    DWORD id = 0;

    av_log_set_level(AV_LOG_INFO); // Set log level to info for output

    DebugPrintfA("BLINXWRAP: Listing all FFmpeg codecs bundled:\n");
    DebugPrintfA("BLINXWRAP: --------------------------------------------------------------------------------\n");
    DebugPrintfA("BLINXWRAP: %-5s %-20s %-40s %s\n", "Type", "Name", "Long Name", "Capabilities");
    DebugPrintfA("BLINXWRAP: --------------------------------------------------------------------------------\n");

    while ((codec = av_codec_iterate(&i))) {
        memset(capabilities, 0, sizeof(capabilities) / sizeof(char));

        // Determine codec type
        const char *type_str;
        switch (codec->type) {
            case AVMEDIA_TYPE_VIDEO:
                type_str = "Video";
                break;
            case AVMEDIA_TYPE_AUDIO:
                type_str = "Audio";
                break;
            case AVMEDIA_TYPE_SUBTITLE:
                type_str = "Sub";
                break;
            case AVMEDIA_TYPE_DATA:
                type_str = "Data";
                break;
            case AVMEDIA_TYPE_ATTACHMENT:
                type_str = "Att";
                break;
            default:
                type_str = "Other";
                break;
        }

        // Skip "Sub" codec type
        if (codec->type == AVMEDIA_TYPE_SUBTITLE)
            continue;

        // Determine additional capabilities
        if (codec->capabilities & AV_CODEC_CAP_DELAY) {
            // Needs a delay for output
            strcat_s(capabilities, AV_PRINT_NAME_BUFFER, " DELAY");
        }
        if (codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) {
            // Variable frame size
            strcat_s(capabilities, AV_PRINT_NAME_BUFFER, " VAR_FS");
        }
        if (codec->capabilities & AV_CODEC_CAP_FRAME_THREADS) {
            strcat_s(capabilities, AV_PRINT_NAME_BUFFER, " FRAME_TRD");
        }
        if (codec->capabilities & AV_CODEC_CAP_HARDWARE) {
            strcat_s(capabilities, AV_PRINT_NAME_BUFFER, " HW_ACCEL");
        }
        if (codec->capabilities & AV_CODEC_CAP_HYBRID) {
            strcat_s(capabilities, AV_PRINT_NAME_BUFFER, " HYBRID_ACCEL");
        }

        DebugPrintfA("BLINXWRAP: %i %-5s %-20s %-40s %s\n", id, type_str, codec->name, codec->long_name ? codec->long_name : "N/A", capabilities);
        id += 1;
    }
    DebugPrintfA("BLINXWRAP: === End ===");
}