cmake_minimum_required(VERSION 3.27)

MESSAGE(STATUS "Boost_INCLUDE_DIR:${Boost_INCLUDE_DIR}")

PROJECT(arrowlib)

#设定编译参数
INCLUDE(cmake-env/complie.cmake)

#生成版本号文件
INCLUDE(cmake-env/version.cmake)

#获取目标平台
INCLUDE(cmake-env/get_target_complate.cmake)

#使用log4cplus库
add_definitions(-DUSE_SDKLOG=1)

IF(${target_platform} STREQUAL "x64")
    INCLUDE(cmake-env/x64-env.cmake)
ELSEIF(${target_platform} STREQUAL "aarch64")
	
ELSEIF(${target_platform} STREQUAL "arm")
    
ELSE(${target_platform})
    MESSAGE(STATUS "* COMPILE_PLATFROM:  Unkonw ${COMPILE_PLATFROM}")
ENDIF()

#插件编译(主要都是用来测试的插件)
ADD_SUBDIRECTORY(test/testmain testmain)

ADD_SUBDIRECTORY(test/testlib1 testlib1)

ADD_SUBDIRECTORY(test/testlib2 testlib2)

