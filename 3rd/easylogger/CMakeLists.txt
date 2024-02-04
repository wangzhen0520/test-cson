cmake_minimum_required(VERSION 3.10)

project(easylogger)

include_directories(
    ./inc
)

aux_source_directory(./src LOG_SRCS)
aux_source_directory(./port LOG_SRCS)

add_library(${PROJECT_NAME} STATIC ${LOG_SRCS})


target_include_directories(${PROJECT_NAME} PUBLIC easylogger)
