#include <jni.h>

#include "logger.hpp"
#include "j4a/JString.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    LOGD("JNI_OnLoad");

    JNIEnv *env = nullptr;
    if (jvm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LOGD("GetEnv JNI_VERSION_1_6 failed");
        return -1;
    }
    LOGD("GetEnv JNI_VERSION_1_6 success");

    dai::j4a::gJString = new dai::j4a::JString(jvm);

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    LOGD("JNI_OnUnload");
    if(dai::j4a::gJString)
    {
        delete dai::j4a::gJString;
        dai::j4a::gJString = nullptr;
    }
}
