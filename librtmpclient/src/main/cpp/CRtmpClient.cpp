
#include "CRtmpClient.hpp"
#include "logger.hpp"

NS_BEGIN

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

    RtmpClientException exceptions;

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

    int CRtmpClient::read(uint8_t* data, int size)
    {
        RTMP_OBJ_CHECK_RETURN(rtmp, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);

        // TODO
    }


NS_END

