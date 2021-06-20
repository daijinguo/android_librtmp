#include <jni.h>
#include <cstring>
#include <string>

#include "rtmp_client.h"

// dai.android.media.live.rtmp
extern "C"
JNIEXPORT jlong JNICALL
Java_dai_android_media_live_rtmp_RtmpClient_nativeConnect(
        JNIEnv *env, jobject thiz, jstring url, jboolean publish) {
    const char *url_c = env->GetStringUTFChars(url, nullptr);
    auto *client = new RtmpClient(url_c);
    client->connect(publish);
    env->ReleaseStringUTFChars(url, url_c);
    return (jlong) client;
}


extern "C"
JNIEXPORT void JNICALL
Java_dai_android_media_live_rtmp_RtmpClient_nativeAudioWriteRawFrame(
        JNIEnv *env, jobject thiz,
        jlong pointer,
        jint sound_format,
        jint sound_rate,
        jint sound_size,
        jint sound_type,
        jint timestamp,
        jbyteArray raw,
        jint length) {

    auto *rtmp = reinterpret_cast<RtmpClient *>(pointer);
    jbyte *buffer = env->GetByteArrayElements(raw, nullptr);
    rtmp->audioRawFrameWrite(sound_format, sound_rate, sound_size, sound_type,
                             reinterpret_cast<char *>(buffer), length,
                             timestamp);
    env->ReleaseByteArrayElements(raw, buffer, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_dai_android_media_live_rtmp_RtmpClient_nativeVideoH264WriteRawFrame(
        JNIEnv *env, jobject thiz,
        jlong pointer,
        jbyteArray raw,
        jint length, jlong dts,
        jlong pts) {
    auto *rtmp = reinterpret_cast<RtmpClient *>(pointer);
    jbyte *buffer = env->GetByteArrayElements(raw, nullptr);
    rtmp->videoH264RawFramesWrite(reinterpret_cast<char *>(buffer), length, dts, pts);
    env->ReleaseByteArrayElements(raw, buffer, 0);
}


extern "C"
JNIEXPORT void JNICALL
Java_dai_android_media_live_rtmp_RtmpClient_nativeRelease(
        JNIEnv *env, jobject thiz, jlong pointer) {
    auto *rtmp = reinterpret_cast<RtmpClient *>(pointer);
    if (!rtmp) {
        delete rtmp;
    }
}