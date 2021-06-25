
#include "CRtmpClient.hpp"
#include "logger.hpp"

NS_BEGIN

    RtmpClientException exceptions;

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
#define RTMP_OBJ_CHECK(obj)                               \
    do{                                                   \
        if ((obj) == nullptr) {                           \
            LOGE("%s: rtmp not allocate.", __FUNCTION__); \
            return;                                       \
        }                                                 \
    } while(0)

#define RTMP_OBJ_CHECK_RETURN(obj, value)                 \
    do {                                                  \
        if( nullptr == (obj) ) {                          \
            LOGE("%s: rtmp not allocate.", __FUNCTION__); \
            return (value);                               \
        }                                                 \
    } while(0)


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
#define AVC_DEFINE(str) static const AVal AV_##str = AVC(#str)
    AVC_DEFINE(onMetaData);
    AVC_DEFINE(duration);
    AVC_DEFINE(width);
    AVC_DEFINE(height);
    AVC_DEFINE(videocodecid);
    AVC_DEFINE(avcprofile);
    AVC_DEFINE(avclevel);
    AVC_DEFINE(videoframerate);
    AVC_DEFINE(audiocodecid);
    AVC_DEFINE(audiosamplerate);
    AVC_DEFINE(audiochannels);
    AVC_DEFINE(avc1);
    AVC_DEFINE(mp4a);
    AVC_DEFINE(onPrivateData);
    AVC_DEFINE(record);

    static const int AAC_ADTS_HEADER_SIZE = 7;
    static const int FLV_TAG_HEAD_LEN = 11;
    static const int FLV_PRE_TAG_LEN = 4;

//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//


    static uint8_t generateAudioTagHeader(
            uint8_t soundFormat, // sound format
            uint8_t soundType,
            uint8_t soundRate)
    {
        // todo

        return 0;
    }


    CRtmpClient::CRtmpClient() :
            enableWrite{ false }, rtmp{ RTMP_Alloc() }
    {
        LOGI("CRtmpClient:%p rtmp:%p", this, rtmp);
    }

    CRtmpClient::~CRtmpClient()
    {
        release();
    }

    void CRtmpClient::release()
    {
        if (rtmp)
        {
            RTMP_Close(rtmp);
            RTMP_Free(rtmp);
            rtmp = nullptr;
            LOGI("CRtmpClient:%p released", this);
        }
    }

    bool CRtmpClient::isConnected()
    {
        RTMP_OBJ_CHECK_RETURN(rtmp, false);

        if (RTMP_IsConnected(rtmp) > 0)
        {
            return true;
        }

        return false;
    }

    int CRtmpClient::openUrl(const char* cUrl, int timeout, bool upload)
    {
        RTMP_OBJ_CHECK_RETURN(rtmp, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);
        url.assign(cUrl);

        RTMP_Init(rtmp);
        rtmp->Link.timeout = timeout;
        if (TRUE != RTMP_SetupURL(rtmp, (char*)cUrl))
        {
            LOGE("%p->%s:%d rtmp set up url failed", this, __FUNCTION__, __LINE__);
            release();
            return exceptions.RTMP_ERROR_URL_SETUP;
        }

        if (upload)
        {
            RTMP_EnableWrite(rtmp);
        }

        if (TRUE != RTMP_Connect(rtmp, nullptr))
        {
            LOGE("%p->%s:%d rtmp connect failed", this, __FUNCTION__, __LINE__);
            release();
            return exceptions.RTMP_ERROR_URL_CONNECT;
        }

        if (TRUE != RTMP_ConnectStream(rtmp, 0))
        {
            LOGE("%p->%s:%d rtmp connect stream failed", this, __FUNCTION__, __LINE__);
            release();
            return exceptions.RTMP_ERROR_URL_CONNECT_STREAM;
        }

        return exceptions.RTMP_TRUE;
    }

    int CRtmpClient::readRaw(uint8_t* data, int size)
    {
        RTMP_OBJ_CHECK_RETURN(rtmp, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);

        // TODO
        return 0;
    }

    int CRtmpClient::writeRaw(uint8_t* data, int size)
    {
        RTMP_OBJ_CHECK_RETURN(rtmp, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);
        // todo
        return 0;
    }

    int CRtmpClient::pause(bool is)
    {
        RTMP_OBJ_CHECK_RETURN(rtmp, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);

        return RTMP_Pause(rtmp, is ? 1 : 0);
    }

    int CRtmpClient::writeFLVHeader(int videoWidth, int videoHeight)
    {
        RTMP_OBJ_CHECK_RETURN(rtmp, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);

        //todo
        return 0;
    }


NS_END

