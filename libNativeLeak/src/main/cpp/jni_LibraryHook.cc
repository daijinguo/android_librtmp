#include "jni_LibraryHook.hpp"
#include "logger.hpp"


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
    return JNI_TRUE;
}

static const char *const c_class = "dai/android/leak/LibraryHook";

static JNINativeMethod g_methods[] = {
    {"hookELF", "([Ljava/lang/String;)Z", (void *)hookELF_},
};

void libraryHook_constructor(JNIEnv *env)
{
    jclass clazz = env->FindClass(c_class);
    if (!clazz) {
        LOGE("can not find class named: %s.", c_class);
        return;
    }

    env->RegisterNatives(clazz, g_methods, sizeof(g_methods) / sizeof(g_methods[0]));
}

void libraryHook_destructor(JNIEnv *env)
{
}
