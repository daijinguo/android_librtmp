cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DNO_CRYPTO)

add_library(rtmp_client STATIC
        amf.c
        hashswf.c
        log.c
        parseurl.c
        rtmp.c
        )

target_link_libraries(rtmp_client log)
