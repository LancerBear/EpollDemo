cmake_minimum_required(VERSION 2.8)

project(EpollDemo)

#编译选项
add_definitions("-Wall -g")
#C++ 11标准，链接pthread
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )

#指定子目录变量
set (COMMON_PROJECT_NAME common)
set (COMMON_DIR ${PROJECT_SOURCE_DIR}/common)
set (COMMON_DIR_SRC ${COMMON_DIR}/src)
set (COMMON_DIR_INC ${COMMON_DIR}/inc)

set (SERVER_PROJECT_NAME server)
set (SERVER_DIR  ${PROJECT_SOURCE_DIR}/server)
set (SERVER_DIR_SRC ${SERVER_DIR}/src)
set (SERVER_DIR_INC ${SERVER_DIR}/inc)

set (CLIENT_PROJECT_NAME client)
set (CLIENT_DIR ${PROJECT_SOURCE_DIR}/client)
set (CLIENT_DIR_SRC ${CLIENT_DIR}/src)
set (CLIENT_DIR_INC ${CLIENT_DIR}/inc)

#指定子目录CMakeLists
add_subdirectory(${COMMON_DIR_SRC})
add_subdirectory(${SERVER_DIR_SRC})
add_subdirectory(${CLIENT_DIR_SRC})

#根目录CMakeLists不生成可执行文件，调用子目录CMakeLists生成
