#pragma once

#include <android/log.h>

#ifndef RTMP_TAG
#define RTMP_TAG "jni_rtmp"
#endif

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  RTMP_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, RTMP_TAG, __VA_ARGS__)
