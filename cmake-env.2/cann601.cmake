cmake_minimum_required(VERSION 3.5)

MESSAGE(STATUS "include cann601.cmake")

##编译环境和参数设置
set(INC_PATH "/usr/local/Ascend/ascend-toolkit/latest/arm64-linux/include/")
# set(LIB_PATH "/usr/local/Ascend/ascend-toolkit/latest/arm64-linux/devlib/")
set(LIB_PATH "/usr/local/Ascend/ascend-toolkit/latest/arm64-linux/runtime/lib64/stub")
set(MAX_PATH "")

#设置预定宏
# add_definitions(-DATLAS500)
add_definitions(-DENABLE_DVPP_INTERFACE)

#设置头文件包含目录
SET(
    ALL_INCLUDE_DIRS
    ${ALL_INCLUDE_DIRS}
    ${INC_PATH}
)

#设置库目录
SET(
    ALL_LIBRARY_DIRS
    ${ALL_LIBRARY_DIRS}
    ${LIB_PATH}
)

#设置链接库
SET(
    ALL_LIBRARIES
    ${ALL_LIBRARIES}
    ascendcl 
    acl_dvpp
)