#include <jni.h>

#include "logger.hpp"
#include "jvm_loader.hpp"

jint JNI_OnLoad(JavaVM* jvm, void* reserved)
{
    JNIEnv* env = nullptr;
    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LOGE("JNI_OnLoad: get 'JNIEnv' failed");
        return JNI_FALSE;
    }

    rtmp_client_OnLoad(env);

    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* jvm, void* reserved)
{
    JNIEnv* env = nullptr;
    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LOGE("JNI_OnUnload: get 'JNIEnv' failed");
        return;
    }

    rtmp_client_OnUnload(env);
}
