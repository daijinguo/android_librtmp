#pragma once

#include <jni.h>
#include <string>

namespace dai::j4a {

class JString {
private:
    JavaVM    *vm;
    jclass     clazz;
    jmethodID  jGetBytes;
    jmethodID  jConstructBytesCharset;
public:
    JString(JavaVM *vm_);
    ~JString();

    jstring createJavaString(JNIEnv *env, const char *buffer, int length);
    jstring createJavaString(JNIEnv *env, std::string &str);
};

extern JString *gJString;


}//namespace dai
