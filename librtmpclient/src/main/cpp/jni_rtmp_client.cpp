#include <jni.h>
#include <cstring>
#include <cinttypes>

#include "librtmp/rtmp.h"
#include "logger.hpp"
#include "jvm_loader.hpp"

#include "CRtmpClient.hpp"

NS_USING

// https://github.com/ant-media/LibRtmp-Client-for-Android.git

namespace
{
    const char* const rtmp_client_java_class = "dai/android/media/client/rtmp/RtmpClient";
    jobject rtmp_class;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    bool videoConfig = false;
    bool audioConfig = false;


}//namespace



static bool exceptionCheck_catchAll(JNIEnv* env)
{
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}

static jobject newGlobalRef_catchAll(JNIEnv* env, jobject clazz)
{
    jobject _global = env->NewGlobalRef(clazz);
    if (!_global || exceptionCheck_catchAll(env))
    {
        return nullptr;
    }
    return _global;
}

static jobject findClassAsGlobalRef_catchAll(JNIEnv* env, const char* sign)
{
    jobject _global = nullptr;
    jobject _local = env->FindClass(sign);
    if (!_local || exceptionCheck_catchAll(env))
    {
        return nullptr;
    }

    _global = newGlobalRef_catchAll(env, _local);
    if (!_global)
    {
        goto FAILED;
    }

FAILED:
    env->DeleteLocalRef(_local);
    return _global;
}

static jint throwIllegalStateException(JNIEnv* env, const char* message)
{
    jclass exception = env->FindClass("java/lang/IllegalStateException");
    return env->ThrowNew(exception, message);
}

static void initRtmpClientException(JNIEnv* env)
{
    rtmp_class = findClassAsGlobalRef_catchAll(env, rtmp_client_java_class);
    if (!rtmp_class)
    {
        LOGE("find and new global class failed");
        return;
    }
    LOGI("Global class: %s", rtmp_client_java_class);

#define GET_STATIC_JAVA_VALUE(name)                                                    \
    do {                                                                               \
        jfieldID field =       env->GetStaticFieldID ((jclass)rtmp_class, #name, "I"); \
        jint     value = (jint)env->GetStaticIntField((jclass)rtmp_class, field);      \
        LOGI("value: %-5d <--> key: %s", value, #name);                                \
        size_t off = offsetof(struct RtmpClientException, name);                       \
        *(jint*)( (char*)(&exceptions) + off ) = value;                                \
    } while(0)

    GET_STATIC_JAVA_VALUE(RTMP_TRUE);
    GET_STATIC_JAVA_VALUE(RTMP_FALSE);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_OBJECT_NOT_ALLOC);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_SETUP);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_CONNECT);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_CONNECT_STREAM);
    GET_STATIC_JAVA_VALUE(RTMP_NOT_CONNECTED);

}

static jlong _init(JNIEnv* env, jobject thiz)
{
    auto* client = new CRtmpClient;
    if (client == nullptr)
    {
        throwIllegalStateException(env, "_init: new CRtmpClient failed");
        return exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC;
    }

    LOGI("_init: CRtmpClient=%p", client);
    return (jlong)client;
}

#define RTMP_CLIENT_CHECK(client, handler)                               \
    auto *(client) = (CRtmpClient*)(handler);                            \
    do{                                                                  \
        if ((client) == nullptr) {                                       \
            LOGE("%s: CRtmpClient not allocate.", __FUNCTION__);         \
            throwIllegalStateException(env, "CRtmpClient not allocate"); \
            return;                                                      \
        }                                                                \
    } while(0)

#define RTMP_CLIENT_CHECK_RETURN(client, handler, value)                \
    auto *(client) = (CRtmpClient*)(handler);                           \
    do {                                                                \
        if(nullptr == (client)) {                                       \
           LOGE("%s: CRtmpClient not allocate.", __FUNCTION__);         \
           throwIllegalStateException(env, "CRtmpClient not allocate"); \
           return (value);                                              \
        }                                                               \
    } while(0)

static jint
_open(JNIEnv* env, jobject thiz, jlong handler,
        jstring url, jboolean publish, jint timeout,
        jobject config)
{
    RTMP_CLIENT_CHECK_RETURN(rtmpClient, handler, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);

    const char* cUrl = env->GetStringUTFChars(url, nullptr);
    int result = rtmpClient->openUrl(cUrl, timeout, publish >= 1);
    env->ReleaseStringUTFChars(url, cUrl);
    return result;
}

static jint _read(JNIEnv* env, jobject thiz, jlong handler, jbyteArray data, jint offset, jint size)
{
    RTMP_CLIENT_CHECK_RETURN(rtmpClient, handler, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);

    if (!rtmpClient->isConnected())
    {
        LOGI("%s:%d rtmp not connected", __FUNCTION__, __LINE__);
        return exceptions.RTMP_NOT_CONNECTED;
    }

    char* buffer = new char[size];
    int result = rtmpClient->readRaw(reinterpret_cast<uint8_t*>(buffer), size);
    if (result > 0)
    {
        env->SetByteArrayRegion(data, offset, result, reinterpret_cast<const jbyte*>(buffer));
    }
    delete[] buffer;
    return result;
}

static jint
_write(JNIEnv* env, jobject thiz, jlong handler, jbyteArray data, jint offset, jint size)
{
    RTMP_CLIENT_CHECK_RETURN(rtmpClient, handler, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);

    if (!rtmpClient->isConnected())
    {
        LOGI("%s:%d rtmp not connected", __FUNCTION__, __LINE__);
        return exceptions.RTMP_NOT_CONNECTED;
    }

    char* buffer = new char[size];
    env->GetByteArrayRegion(data, offset, size, reinterpret_cast<jbyte*>(buffer));
    int result = rtmpClient->writeRaw(reinterpret_cast<uint8_t*>(buffer), size);
    delete[] buffer;
    return result;
}

static jint _pause(JNIEnv* env, jobject thiz, jlong handler, jboolean pause)
{
    RTMP_CLIENT_CHECK_RETURN(rtmpClient, handler, exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC);
    return rtmpClient->pause(pause);
}

static jboolean _isConnected(JNIEnv* env, jobject thiz, jlong handler)
{
    RTMP_CLIENT_CHECK_RETURN(rtmpClient, handler, JNI_FALSE);
    return rtmpClient->isConnected();
}

static void _close(JNIEnv* env, jobject thiz, jlong handler)
{
    RTMP_CLIENT_CHECK(rtmpClient, handler);
    delete rtmpClient;
}


static jint _writeAudio(JNIEnv* env, jobject thiz, jlong handler,
        jbyteArray data, jint offset, jint length, jlong timestamp)
{
    RTMP* rtmp = (RTMP*)handler;
    if (!rtmp)
    {
        LOGE("_writeAudio: bad rtmp object handler");
        throwIllegalStateException(env, "_writeAudio: bad rtmp object handler");
        return exceptions.RTMP_ERROR_OBJECT_NOT_ALLOC;
    }

    jbyte* cdata = env->GetByteArrayElements(data, nullptr);

    int result = 0;
    uint32_t audio_dts = (uint32_t)timestamp;
    uint32_t index = 0;
    uint32_t bodyLength = 0;
    uint32_t outputLength = 0;
    char* output = nullptr;

    if (audioConfig == false)
    {
        bodyLength = 2 + 2; // AudioTagHeader + AudioSpecificConfig
        //outputLength = bodyLength + FLV_TAG_HEAD_LEN + FLV_PRE_TAG_LEN;
        output = new char[outputLength];

        // flv tag header
        output[index++] = 0x08;  // tag type audio

        output[index++] = (uint8_t)(bodyLength >> 16); // data len
        output[index++] = (uint8_t)(bodyLength >> 8);  // data len
        output[index++] = (uint8_t)(bodyLength);       // data len

        output[index++] = (uint8_t)(audio_dts >> 16); // time stamp
        output[index++] = (uint8_t)(audio_dts >> 8);  // time stamp
        output[index++] = (uint8_t)(audio_dts);       // time stamp
        output[index++] = (uint8_t)(audio_dts >> 24); // time stamp

        output[index++] = 0; // stream id 0
        output[index++] = 0; // stream id 0
        output[index++] = 0; // stream id 0

        // flv AudioTagHeader
        {
            // sound format aac
            uint8_t soundType = 1;
            uint8_t soundRate = 3;
            output[index++] = 0xA0 | (soundRate << 2) | 0x02 | soundType;
        }
        output[index++] = 0x01; // aac sequence header


    }

    env->ReleaseByteArrayElements(data, cdata, JNI_ABORT);
}

static JNINativeMethod g_methods[] = {
        { "nativeInit",        "()J",                      (void*)_init },
        { "nativeOpen",        "(JLjava/lang/String;ZI)I", (void*)_open },
        { "nativeRead",        "(J[BII)I",                 (void*)_read },
        { "nativeWrite",       "(J[BII)I",                 (void*)_write },
        { "nativePause",       "(JZ)I",                    (void*)_pause },
        { "nativeIsConnected", "(J)Z",                     (void*)_isConnected },
        { "nativeClose",       "(J)V",                     (void*)_close },
        { "nativeWriteAudio",  "(J[BIIJ)I",                (void*)_writeAudio },
};

void rtmp_client_OnLoad(JNIEnv* env)
{
    initRtmpClientException(env);

    const auto N = sizeof(g_methods) / sizeof(g_methods[0]);
    env->RegisterNatives((jclass)rtmp_class, g_methods, N);
}

void rtmp_client_OnUnload(JNIEnv* env)
{
    env->UnregisterNatives((jclass)rtmp_class);
}


// extern "C" JNIEXPORT jlong JNICALL
// Java_dai_android_media_client_rtmp_RtmpClient_nativeInit(JNIEnv* env, jobject thiz)
// {
//     return 0L;
// }
