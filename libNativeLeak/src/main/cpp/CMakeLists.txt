CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(PROJECT_NAME "native_hooker")

PROJECT(${PROJECT_NAME})

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
)

ADD_LIBRARY(
        ${PROJECT_NAME} SHARED
        android_main.cc
        jni_LibraryHook.cc
        j4a/JString.cc
        hook/Core.cc
        hook/Stack.cc
        core/Looper.cc
)

TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        log
)
