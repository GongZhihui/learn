#CMAKE_PREFIX_PATH          find_package的搜索目录
#EXECUTABLE_OUTPUT_PATH     exe的输出目录
#LIBRARY_OUTPUT_PATH        lib,dll的输出目录
#include_directories        对应vs中的附加包含目录
#link_directories           对应vs中的附加库目录
#link_libraries             对应vs中的附加依赖项，它一定得在add_executable或add_library之前
#add_executable             生成exe程序add_library生成lib或dll库find_package如果第三次库提供了xxxx.cmake，配置CMAKE_PREFIX_PATH可找到include，lib等
#target_link_libraries       它一定得在add_executable或add_library之后
#add_subdirectory(xxx)      Xxx目录下必须存在一个CMakeLists.txt


cmake_minimum_required(VERSION 3.5)
project(learn LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 定义项目根目录
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#定义exe, lib, dll的安装目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/target/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/target/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/target/lib)

#设置兼容xp
#set(CMAKE_GENERATOR_TOOLSET "v140_xp" CACHE STRING "Platform Toolset" FORCE)

#设置debug版本生成文件后缀
set(CMAKE_DEBUG_POSTFIX "d")

#添加子项目
add_subdirectory(ffmpeg)
add_subdirectory(d3dwindows)

#创建项目分类文件夹
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
#set_target_properties(widget PROPERTIES FOLDER widget)
#set_target_properties(helloworld PROPERTIES FOLDER hello/world)


