# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (basis)

# 设定路径
set(PROJECT_ROOT_PATH "${CMAKE_SOURCE_DIR}/../")    		# 工程的根目录
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/")		# 可执行文件输出目录
set(LIB_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/")				# 静态链接库输出目录
set(DEPEND_LIB_PATH "${LIB_OUTPUT_PATH}")

# 头文件路径
include_directories("${PROJECT_ROOT_PATH}/include/")

# 库文件路径
link_directories(${DEPEND_LIB_PATH})

# 增加CMakeLists子目录
add_subdirectory(src)