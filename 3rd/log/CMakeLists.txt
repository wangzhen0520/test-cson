cmake_minimum_required(VERSION 3.10)

project(log.c)

include_directories(
    ./log.c/src
)

aux_source_directory(./log.c/src LOG_SRCS)

add_library(${PROJECT_NAME} STATIC ${LOG_SRCS})


target_include_directories(${PROJECT_NAME} PUBLIC log.c)
