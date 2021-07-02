#pragma once

#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TAG "NativeLeakHook"

#define LOG(level, fmt, ...)                                                                                              \
    do{                                                                                                                   \
        __android_log_print(level, LOG_TAG, "[%s:%d@%s]>>> " fmt, __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__ ); \
    } while(0)

#define LOGV(fmt, ...) LOG(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) LOG(ANDROID_LOG_DEBUG,   fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) LOG(ANDROID_LOG_INFO,    fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG(ANDROID_LOG_WARN,    fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) LOG(ANDROID_LOG_ERROR,   fmt, ##__VA_ARGS__)
#define LOGF(fmt, ...) LOG(ANDROID_LOG_FATAL,   fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif


#include <string>
#include <vector>
#include <cstdio>

template <typename ...Args>
std::string string_format(const char *fmt, Args... args)
{
    constexpr size_t old_length = BUFSIZ;
    char buffer[old_length] = {0x00};
    size_t new_length = snprintf(&buffer[0], old_length, fmt, args...);
    new_length++;

    if(new_length > old_length)
    {
        std::vector<char> newBuffer(new_length);
        snprintf(newBuffer.data(), new_length, fmt, args...);
        return std::string(newBuffer.data());
    }
    return buffer;
}



