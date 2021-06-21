#include <jni.h>

#include "librtmp/rtmp.h"
#include "logger.hpp"
#include "jvm_loader.hpp"

// https://github.com/ant-media/LibRtmp-Client-for-Android.git

namespace
{
    const char* const rtmp_client_java_class = "dai/android/media/client/rtmp/RtmpClient";

    struct RtmpClientException
    {
        jint RTMP_TRUE;
        jint RTMP_FALSE;

        jint RTMP_ERROR_MEM_ALLOC;

        jint RTMP_ERROR_URL_INIT;
        jint RTMP_ERROR_URL_CONNECT;
        jint RTMP_ERROR_URL_CONNECT_STREAM;

        jint RTMP_SUCCESS;
        jint RTMP_ERROR_OPEN_ALLOC;
        jint RTMP_ERROR_OPEN_CONNECT_STREAM;
        jint RTMP_ERROR_UNKNOWN_RTMP_OPTION;
        jint RTMP_ERROR_UNKNOWN_RTMP_AMF_TYPE;
        jint RTMP_ERROR_DNS_NOT_REACHABLE;
        jint RTMP_ERROR_SOCKET_CONNECT_FAIL;
        jint RTMP_ERROR_SOCKS_NEGOTIATION_FAIL;
        jint RTMP_ERROR_SOCKET_CREATE_FAIL;
        jint RTMP_ERROR_NO_SSL_TLS_SUPP;
        jint RTMP_ERROR_HANDSHAKE_CONNECT_FAIL;
        jint RTMP_ERROR_HANDSHAKE_FAIL;
        jint RTMP_ERROR_CONNECT_FAIL;
        jint RTMP_ERROR_CONNECTION_LOST;
        jint RTMP_ERROR_KEYFRAME_TS_MISMATCH;
        jint RTMP_ERROR_READ_CORRUPT_STREAM;
        jint RTMP_ERROR_MEM_ALLOC_FAIL;
        jint RTMP_ERROR_STREAM_BAD_DATASIZE;
        jint RTMP_ERROR_PACKET_TOO_SMALL;
        jint RTMP_ERROR_SEND_PACKET_FAIL;
        jint RTMP_ERROR_AMF_ENCODE_FAIL;
        jint RTMP_ERROR_URL_MISSING_PROTOCOL;
        jint RTMP_ERROR_URL_MISSING_HOSTNAME;
        jint RTMP_ERROR_URL_INCORRECT_PORT;
        jint RTMP_ERROR_IGNORED;
        jint RTMP_ERROR_GENERIC_ERROR;
        jint RTMP_ERROR_SANITY_FAIL;
    };

    struct RtmpClientClass
    {
        constexpr static const char* const c_class = "dai/android/media/client/rtmp/RtmpClient";

        jobject clazz;

        RtmpClientException exceptions;
    };

    RtmpClientClass rtmpClientClass;
} // namespace

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
    rtmpClientClass.clazz = findClassAsGlobalRef_catchAll(env, RtmpClientClass::c_class);
    if (!rtmpClientClass.clazz)
    {
        LOGE("find and new global class failed");
        return;
    }
    LOGI("Global class: %s", RtmpClientClass::c_class);

#define GET_STATIC_JAVA_VALUE(name)                                                               \
    do {                                                                                          \
        jfieldID field =       env->GetStaticFieldID ((jclass)rtmpClientClass.clazz, #name, "I"); \
        jint     value = (jint)env->GetStaticIntField((jclass)rtmpClientClass.clazz, field);      \
        LOGI("value: %-5d <--> key: %s", value, #name);                                           \
        size_t off = offsetof(struct RtmpClientException, name);                                  \
        *(jint*)( (char*)(&rtmpClientClass.exceptions) + off ) = value;                           \
    } while(0)

    GET_STATIC_JAVA_VALUE(RTMP_TRUE);
    GET_STATIC_JAVA_VALUE(RTMP_FALSE);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_MEM_ALLOC);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_INIT);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_CONNECT);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_CONNECT_STREAM);


    GET_STATIC_JAVA_VALUE(RTMP_SUCCESS);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_OPEN_ALLOC);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_OPEN_CONNECT_STREAM);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_UNKNOWN_RTMP_OPTION);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_UNKNOWN_RTMP_AMF_TYPE);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_DNS_NOT_REACHABLE);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_SOCKET_CONNECT_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_SOCKS_NEGOTIATION_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_SOCKET_CREATE_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_NO_SSL_TLS_SUPP);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_HANDSHAKE_CONNECT_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_HANDSHAKE_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_CONNECT_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_CONNECTION_LOST);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_KEYFRAME_TS_MISMATCH);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_READ_CORRUPT_STREAM);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_MEM_ALLOC_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_STREAM_BAD_DATASIZE);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_PACKET_TOO_SMALL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_SEND_PACKET_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_AMF_ENCODE_FAIL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_MISSING_PROTOCOL);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_MISSING_HOSTNAME);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_URL_INCORRECT_PORT);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_IGNORED);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_GENERIC_ERROR);
    GET_STATIC_JAVA_VALUE(RTMP_ERROR_SANITY_FAIL);
}

static jlong _init(JNIEnv* env, jobject thiz)
{
    RTMP* rtmp = RTMP_Alloc();
    if (!rtmp)
    {
        throwIllegalStateException(env, "allocate rtmp failed");
    }

    LOGI("rtmp address=%p", rtmp);
    return (jlong)rtmp;
}

static jint
_open(JNIEnv* env, jobject thiz, jlong handler, jstring url, jboolean publish, jint timeout)
{
    RTMP* rtmp = (RTMP*)handler;
    if (!rtmp)
    {
        LOGE("_open: bad rtmp object handler");
        throwIllegalStateException(env, "bad rtmp object handler");
        return rtmpClientClass.exceptions.RTMP_ERROR_MEM_ALLOC;
    }

    const char* cUrl = env->GetStringUTFChars(url, nullptr);
    int result = rtmpClientClass.exceptions.RTMP_TRUE;

    RTMP_Init(rtmp);
    rtmp->Link.timeout = timeout;
    result = RTMP_SetupURL(rtmp, (char*)cUrl);
    if (result != TRUE)
    {
        LOGE("_open: setup rtmp url failed, reason=%d", result);
        RTMP_Free(rtmp);
        result = rtmpClientClass.exceptions.RTMP_ERROR_URL_INIT;
        goto FAILED;
    }

    if (publish)
    {
        RTMP_EnableWrite(rtmp);
    }

    result = RTMP_Connect(rtmp, nullptr);
    if (result != TRUE)
    {
        LOGE("_open: rtmp connect failed, reason=%d", result);
        RTMP_Free(rtmp);
        result = rtmpClientClass.exceptions.RTMP_ERROR_URL_CONNECT;
        goto FAILED;
    }

    result = RTMP_ConnectStream(rtmp, 0);
    if (result <= 0)
    {
        LOGE("_open: rtmp connect stream failed, reason=%d", result);
        RTMP_Free(rtmp);
        result = rtmpClientClass.exceptions.RTMP_ERROR_URL_CONNECT_STREAM;
        goto FAILED;
    }

FAILED:
    env->ReleaseStringUTFChars(url, cUrl);
    return result;
}

static jint _read(JNIEnv* env, jobject thiz, jlong handler, jbyteArray data, jint offset, jint size)
{
    RTMP* rtmp = (RTMP*)handler;
    if (!rtmp)
    {
        LOGE("_read: bad rtmp object handler");
        throwIllegalStateException(env, "bad rtmp object handler");
        return rtmpClientClass.exceptions.RTMP_ERROR_MEM_ALLOC;
    }

    int connected = RTMP_IsConnected(rtmp);
    if (connected != TRUE)
    {
        LOGE("_read: rtmp not connected");
        return rtmpClientClass.exceptions.RTMP_ERROR_URL_CONNECT;
    }

    char* c_data = new char[size];
    int iRead = RTMP_Read(rtmp, c_data, size);
    if (iRead > 0)
    {
        env->SetByteArrayRegion(data, offset, iRead, (const jbyte*)c_data);
    }

    delete[] c_data;
    return iRead;
}

static jint
_write(JNIEnv* env, jobject thiz, jlong handler, jbyteArray data, jint offset, jint size)
{
    // TODO: implement nativeWrite()
}

static jint _pause(JNIEnv* env, jobject thiz, jlong handler, jboolean pause)
{
    // TODO: implement nativePause()
}

static jboolean _isConnected(JNIEnv* env, jobject thiz, jlong handler)
{
    // TODO: implement nativeIsConnected()
}

static void _close(JNIEnv* env, jobject thiz, jlong handler)
{
    // TODO: implement nativeClose()
}

static JNINativeMethod g_methods[] = {
    { "nativeInit", "()J", (void*)_init },
    { "nativeOpen", "(JLjava/lang/String;ZI)I", (void*)_open },
    { "nativeRead", "(J[BII)I", (void*)_read },
    { "nativeWrite", "(J[BII)I", (void*)_write },
    { "nativePause", "(JZ)I", (void*)_pause },
    { "nativeIsConnected", "(J)Z", (void*)_isConnected },
    { "nativeClose", "(J)V", (void*)_close },
};

void rtmp_client_OnLoad(JNIEnv* env)
{
    initRtmpClientException(env);

    const auto N = sizeof(g_methods) / sizeof(g_methods[0]);
    env->RegisterNatives((jclass)rtmpClientClass.clazz, g_methods, N);
}

void rtmp_client_OnUnload(JNIEnv* env)
{
    env->UnregisterNatives((jclass)rtmpClientClass.clazz);
}

