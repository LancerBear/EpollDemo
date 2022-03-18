project(${SERVER_PROJECT_NAME})

#cpp源文件目录
aux_source_directory(${SERVER_DIR_SRC} SOURCE_FILES)

#头文件依赖common
include_directories(${SERVER_DIR_INC} ${COMMON_DIR_INC})

# #依赖链接common
# link_libraries(${COMMON_DIR_SRC})

#可执行文件server
add_executable(${SERVER_PROJECT_NAME} ${SOURCE_FILES})

#链接libcommon.a
target_link_libraries(${SERVER_PROJECT_NAME} ${COMMON_PROJECT_NAME})