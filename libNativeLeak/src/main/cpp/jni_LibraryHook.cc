#include "jni_LibraryHook.hpp"
#include "logger.hpp"

#include <jni.h>

#include <list>
#include <string>


//!!  extern "C"
//!!  JNIEXPORT void JNICALL
//!!  Java_dai_android_leak_LibraryHook_init(JNIEnv *env, jclass clazz)
//!!  {
//!!  }

//!! extern "C"
//!! JNIEXPORT jboolean JNICALL
//!! Java_dai_android_leak_LibraryHook_hookELF(JNIEnv *env, jclass clazz, jobjectArray files)
//!! {
//!!     // TODO: implement hookELF()
//!! }

jboolean hookELF_(JNIEnv *env, jclass clazz, jobjectArray files)
{
    jsize length = env->GetArrayLength(files);

    LOGI("elf file size: %d", length);

    std::vector<std::string> libs(length);
    for (int i = 0; i < length; ++i)
    {
        auto libName = (jstring)env->GetObjectArrayElement(files, i);
        const char *cstr = env->GetStringUTFChars(libName, nullptr);
        libs.emplace_back(cstr);
        LOGI("library item name: %s", cstr);
        env->ReleaseStringUTFChars(libName, cstr);
    }

    if(libs.empty())
    {
        LOGW("null or empty elf file list");
        return JNI_FALSE;
    }

    for(const auto &item : libs)
    {
        const char *elf = item.c_str();
        std::string libName = item + ".so";
        std::string soFile = ".*/" + item + "\\.so$";
    }

    return JNI_TRUE;
}

static const char *const c_class = "dai/android/leak/LibraryHook";

static JNINativeMethod g_methods[] =
{
    { "hookELF", "([Ljava/lang/String;)Z", (void *)hookELF_ },
};

void libraryHook_constructor(JNIEnv *env)
{
    LOGD("constructor");
    jclass clazz = env->FindClass(c_class);
    if (!clazz)
    {
        LOGE("can not find class named: %s.", c_class);
        return;
    }
    LOGI("find class named: %s.", c_class);

    env->RegisterNatives(clazz, g_methods, sizeof(g_methods) / sizeof(g_methods[0]));
}

void libraryHook_destructor(JNIEnv *env)
{
    LOGD("destructor");
}
