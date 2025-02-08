#华为安红安全库
cmake_minimum_required(VERSION 3.5)

MESSAGE(STATUS "include huawei_secure.cmake")

#设置预定宏（按需修改）
add_definitions(-DHUAWEI_SECURE)

#设置库目录
SET(
    ALL_LIBRARY_DIRS
    ${ALL_LIBRARY_DIRS}
    ${CMAKE_SOURCE_DIR}/pipeline_plugin_sdk/safe_fun/huawei_securcec/lib
)

#设置链接库
SET(
    ALL_LIBRARIES
    ${ALL_LIBRARIES}
    libsecurec.so
)
