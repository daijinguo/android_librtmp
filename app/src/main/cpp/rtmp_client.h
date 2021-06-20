
#ifndef KU_LIVE_RTMP_CLIENT_H_INCLUDE
#define KU_LIVE_RTMP_CLIENT_H_INCLUDE

#include "Logger.h"
#include "srs_librtmp.h"


#ifdef NDK_LOG_TAG
#undef NDK_LOG_TAG
#define NDK_LOG_TAG "NDK_RTMP_CLIENT"
#endif


class RtmpClient {
private:
    srs_rtmp_t rtmp;

public:
    explicit RtmpClient(const char *url_);

    ~RtmpClient();

    void connect(bool publish);

    int audioRawFrameWrite(
            char format, char rate, char size, char type,
            char *frame, int frame_size,
            u_int32_t timestamp);

    int videoH264RawFramesWrite(char *frames, int frames_size, u_int32_t dts, u_int32_t pts);

};


#endif //KU_LIVE_RTMP_CLIENT_H_INCLUDE

