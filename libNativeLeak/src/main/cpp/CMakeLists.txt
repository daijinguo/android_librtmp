CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)

SET(PROJECT_NAME "NativeLeakReporter")

PROJECT(${PROJECT_NAME})

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
)

ADD_LIBRARY(
        ${PROJECT_NAME} SHARED
        android_main.cc
        j4a/JString.cc
        hook/Core.cc
)

TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        log
)
