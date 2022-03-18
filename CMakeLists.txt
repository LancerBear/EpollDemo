project(Epoll)

#编译选项
add_definitions("-Wall -lpthread -g")

#指定子目录变量
set (SERVER_DIR ${PROJECT_SOURCE_DIR/server})
set (CILENT_DIR ${PROJECT_SOURCE_DIR/client})

#指定子目录CMakeLists
add_subdirectory(server)
add_subdirectory(client)
