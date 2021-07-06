#pragma once

#include <jni.h>

void libraryHook_constructor(JNIEnv *env);

void libraryHook_destructor(JNIEnv *env);
