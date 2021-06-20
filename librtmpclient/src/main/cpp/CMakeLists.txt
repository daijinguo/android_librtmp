cmake_minimum_required(VERSION 3.4.1)

set(PROJECT_NAME "rtmp_client_jni")

add_subdirectory(librtmp)


add_library(
        ${PROJECT_NAME} SHARED
        jni_rtmp_client.cpp
)

target_link_libraries(
        ${PROJECT_NAME}
        rtmp_client
        log
)
