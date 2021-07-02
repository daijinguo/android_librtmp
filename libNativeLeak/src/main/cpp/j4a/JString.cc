#include "JString.hpp"
#include "../logger.hpp"

namespace dai::j4a {

[[maybe_unused]] JString *gJString = nullptr;

JString::JString(JavaVM *vm_) : vm{ vm_ }
{
    JNIEnv *env = nullptr;
    vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (nullptr == env)
    {
        LOGF("GetEnv: JNI_VERSION_1_6 failed");
        return;
    }

    jclass local = env->FindClass("java/lang/String");
    if (nullptr == local)
    {
        LOGF("FindClass: java/lang/String failed");
        return;
    }

    clazz                  = (jclass)env->NewGlobalRef(local);
    jConstructBytesCharset = env->GetMethodID(clazz, "<init>",   "([BLjava/lang/String;)V");
    jGetBytes              = env->GetMethodID(clazz, "getBytes", "(Ljava/lang/String;)[B");

    LOGI("constructor success");
}

JString::~JString()
{
    if (!vm)
    {
        LOGE("null object JavaVM");
        return;
    }

    JNIEnv *env = nullptr;
    vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (nullptr == env)
    {
        LOGE("GetEnv: JNI_VERSION_1_6 failed");
        return;
    }

    env->DeleteGlobalRef(clazz);
    vm = nullptr;
}

jstring JString::createJavaString(JNIEnv *env, std::string &str)
{
    return createJavaString(env, str.c_str(), str.length());
}

jstring JString::createJavaString(JNIEnv *env, const char *buffer, int length)
{
    jbyteArray bytes = env->NewByteArray(length);
    env->SetByteArrayRegion(bytes, 0, length, (jbyte *)buffer);
    jstring encoding = env->NewStringUTF("utf-8");
    auto result = (jstring)env->NewObject(clazz, jConstructBytesCharset, bytes, encoding);
    env->DeleteLocalRef(encoding);
    env->DeleteLocalRef(bytes);
    return result;
}

}//namespace dai::j4a
