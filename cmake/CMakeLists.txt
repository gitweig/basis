# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (basis)

# 设定路径
set(PROJECT_ROOT_PATH "${CMAKE_SOURCE_DIR}/../")    		# 工程的根目录
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/")		# 可执行文件输出目录
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/")				# 静态链接库输出目录

# 头文件路径
include_directories("${PROJECT_ROOT_PATH}/include/")

# 库文件路径
link_directories(${DEPEND_LIB_PATH})

# 增加CMakeLists子目录
add_subdirectory(src)


#cmake_minimum_required(VERSION 2.8)
#project(helloworld)

#set(CMAKE_VERBOSE_MAKEFILE on)
#set(CMAKE_CXX_COMPILER "g++")
#set(CMAKE_CXX_FLAGS "-Wall")
#set(CMAKE_CXX_FLAGS_DEBUG "-g3")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2")
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#aux_source_directory(./ SRC_LIST)
#aux_source_directory(./other OTHER_SRC_LIST)
#list(APPEND SRC_LIST ${OTHER_SRC_LIST})

#include_directories(${PROJECT_SOURCE_DIR}/include)
#link_directories(${PROJECT_SOURCE_DIR}/lib)

#if(${CMAKE_BUILD_TYPE} MATCHES "debug")
#    add_executable(hellod ${SRC_LIST})
#    target_link_libraries(hellod Ad Bd.a Cd.so)
#else()
#    add_executable(hello ${SRC_LIST})
#    target_link_libraries(hello A B.a C.so)
#endif()

#message("args print")
#message("CMAKE_BINARY_DIR  ${CMAKE_BINARY_DIR}")
#message("CMAKE_COMMAND  ${CMAKE_COMMAND}")
#message("CMAKE_CURRENT_BINARY_DIR   ${CMAKE_CURRENT_BINARY_DIR}")
#message("CMAKE_CURRENT_LIST_FILE   ${CMAKE_CURRENT_LIST_FILE}")
#message("CMAKE_CURRENT_LIST_DIR  ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_CURRENT_LIST_LINE  ${CMAKE_CURRENT_LIST_LINE}")
#message("CMAKE_CURRENT_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}")
#message("CMAKE_FILES_DIRECTORY  ${CMAKE_FILES_DIRECTORY}")
#message("CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}")
#message("CMAKE_ROOT  ${CMAKE_ROOT}")
#message("CMAKE_SOURCE_DIR  ${CMAKE_SOURCE_DIR}")
#message("EXECUTABLE_OUTPUT_PATH  ${EXECUTABLE_OUTPUT_PATH}")
#message("LIBRARY_OUTPUT_PATH  ${LIBRARY_OUTPUT_PATH}")
#message("PROJECT_NAME  ${PROJECT_NAME}")
#message("CMAKE_PROJECT_NAME  ${CMAKE_PROJECT_NAME}")
#message("PROJECT_BINARY_DIR  ${PROJECT_BINARY_DIR}")
#message("PROJECT_SOURCE_DIR  ${PROJECT_SOURCE_DIR}")

#message("CMAKE_INCLUDE_PATH  ${CMAKE_INCLUDE_PATH}")
#message("CMAKE_LIBRARY_PATH  ${CMAKE_LIBRARY_PATH}")
#message("CMAKE_PREFIX_PATH   ${CMAKE_PREFIX_PATH}")
#message("CMAKE_INSTALL_ALWAYS   ${CMAKE_INSTALL_ALWAYS}")
#message("DESTDIR  ${DESTDIR}")

#message("CMAKE_BUILD_TYPE  ${CMAKE_BUILD_TYPE}")
 