cmake_minimum_required(VERSION 3.10.2)

project("KuLive")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/srs-librtmp)


add_library(
        KuLive SHARED
        android_main.cpp
)


target_link_libraries(
        KuLive

        # included in the NDK.
        log)