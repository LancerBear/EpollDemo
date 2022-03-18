#使用根目录CMakeLists设置的变量
project(${COMMON_PROJECT_NAME})

#设置源文件目录到 SOURCE_FILES变量中
aux_source_directory(${COMMON_DIR_SRC} SOURCE_FILES)

#头文件目录
include_directories(${COMMON_DIR_INC})

#生成静态链接文件
add_library(${COMMON_PROJECT_NAME} STATIC ${SOURCE_FILES})