
#include "rtmp_client.h"

RtmpClient::RtmpClient(const char *url_) {
    rtmp = srs_rtmp_create(url_);
}

RtmpClient::~RtmpClient() {
    if (!rtmp) {
        srs_rtmp_destroy(rtmp);
        rtmp = nullptr;
    }
}

void RtmpClient::connect(bool publish) {
    if (srs_rtmp_handshake(rtmp) != 0) {
        ALOGE("srs_rtmp_handshake failed");
        return;
    }

    if (srs_rtmp_connect_app(rtmp) != 0) {
        ALOGE("srs_rtmp_connect_app failed");
        return;
    }

    if (publish) {
        if (srs_rtmp_publish_stream(rtmp) != 0) {
            ALOGE("srs_rtmp_publish_stream failed");
            return;
        }
    }
}

int RtmpClient::audioRawFrameWrite(
        char format, char rate, char size, char type,
        char *frame, int frame_size,
        u_int32_t timestamp) {
    if (!rtmp) {
        ALOGE("nullptr of rtmp client when 'audio_write_raw_frame'");
        return -1;
    }

    return srs_audio_write_raw_frame(
            rtmp,
            format, rate, size, type,
            frame,
            frame_size, timestamp);
}

int
RtmpClient::videoH264RawFramesWrite(char *frames, int frames_size, u_int32_t dts, u_int32_t pts) {
    if (!rtmp) {
        ALOGE("nullptr of rtmp client when 'h264_write_raw_frames'");
        return -1;
    }

    return srs_h264_write_raw_frames(rtmp, frames, frames_size, dts, pts);
}
