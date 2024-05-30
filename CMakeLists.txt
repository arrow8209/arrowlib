cmake_minimum_required(VERSION 3.27)

PROJECT(arrowlib)

#设定编译参数
INCLUDE(cmake-env/complie.cmake)

#生成版本号文件
INCLUDE(cmake-env/version.cmake)

#获取目标平台
INCLUDE(cmake-env/get_target_complate.cmake)
get_tareget_platform(target_platform)
message(STATUS "target_platform:${target_platform}")

#设置编译环境
SET(host_platform "code_server")
# SET(host_platform "company")
# SET(host_platform "max")

INCLUDE(cmake-env/x64/x64-arrow-server.cmake)

#插件编译(主要都是用来测试的插件)
ADD_SUBDIRECTORY(testmain)

ADD_SUBDIRECTORY(testlib1)

# ADD_SUBDIRECTORY(testlib2)