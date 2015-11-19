
# 确定要编译的文件
aux_source_directory("${PROJECT_ROOT_PATH}/src/" SOURCE)
aux_source_directory("${PROJECT_ROOT_PATH}/include/" HPP)
#file(GLOB SOURCES "${PROJECT_ROOT_PATH}/src/*.cpp") 

add_executable(basis ${HPP} ${SOURCE})   