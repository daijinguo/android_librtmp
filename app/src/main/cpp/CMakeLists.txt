cmake_minimum_required(VERSION 3.10.2)

project("KuLive")

# add_definitions(-DNO_CRYPTO)

# add librtmp source codes and head file
#### librtmp/amf.h
#### librtmp/amf.c
#### librtmp/dh.h
#### librtmp/dhgroups.h
#### librtmp/handshake.h
#### librtmp/hashswf.c
#### librtmp/http.h
#### librtmp/log.h
#### librtmp/log.c
#### librtmp/parseurl.c
#### librtmp/rtmp.h
#### librtmp/rtmp.c
#### librtmp/rtmp_sys.h
#### RtmpClient_jni_1.cpp

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/srs-librtmp)


add_library(
        KuLive SHARED

        srs-librtmp/srs_librtmp.cpp
        android_main.cpp
        rtmp_client.cpp
        rtmp_client_jni.cpp
)


target_link_libraries(
        KuLive

        # included in the NDK.
        log)