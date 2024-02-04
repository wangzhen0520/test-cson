cmake_minimum_required(VERSION 3.10)

project(test)

# 这段代码可以区分操作系统
message("Identifying the OS...")
if(WIN32)
  message("This is Windows.")
elseif(APPLE)
  message("This is MacOS.")
elseif(UNIX)
  message("This is Linux.")
endif()
# 这段代码可以区分操作系统

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")

get_filename_component(CURDIR ${CMAKE_CURRENT_SOURCE_DIR} NAME ABSOLUTE)
set(TRAGET_NAME ${CURDIR})

# add_definitions(-DDEBUG) # 相当于代码中 #define DEBUG

# 设置目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
# 重设目标生成目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# 头文件包含目录
include_directories(
  .
  ./src
  ${CMAKE_CURRENT_SOURCE_DIR}/cson/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/cson/cjson
  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/log/log.c/src
  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/easylogger/inc  
)

# 添加源代码文件夹, 自动扫描所有文件
aux_source_directory(. DIR_SRCS)
aux_source_directory(./src DIR_SRCS)
aux_source_directory(./cson/src DIR_SRCS)
aux_source_directory(./cson/cjson DIR_SRCS)

# 添加子工程
add_subdirectory(3rd)

# link_directories(${LIBRARY_OUTPUT_PATH})

add_executable(${TRAGET_NAME} ${DIR_SRCS})

# 链接子工程
target_link_libraries(${TRAGET_NAME} PRIVATE -lm log.c easylogger)
