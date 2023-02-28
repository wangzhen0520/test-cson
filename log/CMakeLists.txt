cmake_minimum_required(VERSION 3.10)

project(log)

AUX_SOURCE_DIRECTORY(. LOG_SRCS)

add_library(${PROJECT_NAME} STATIC ${LOG_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/log)
