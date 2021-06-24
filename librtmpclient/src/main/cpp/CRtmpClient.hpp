#pragma once

#include <jni.h>
//#include <memory>
#include <string>

#include "namespace.hpp"
#include "librtmp/rtmp.h"


NS_BEGIN

    struct RtmpClientException
    {
        jint RTMP_TRUE;
        jint RTMP_FALSE;

        jint RTMP_ERROR_OBJECT_NOT_ALLOC;

        jint RTMP_ERROR_URL_SETUP;
        jint RTMP_ERROR_URL_CONNECT;
        jint RTMP_ERROR_URL_CONNECT_STREAM;
    };

    extern RtmpClientException exceptions;


    class CRtmpClient
    {
    private:
        RTMP* rtmp;
        bool enableWrite;
        std::string url;

    private:
        void release();

    public:
        CRtmpClient();

        ~CRtmpClient();

    public:
        int openUrl(const char* cUrl, int timeout, bool upload = false);

        int read(uint8_t* data, int size);

        int write();

        int writeVideoH264();

        int writeAudioAcc();

        bool isConnected();

    };


NS_END
