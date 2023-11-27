cmake_minimum_required(VERSION 3.5)

PROJECT(testmain)

MESSAGE("**********************************************************8")
MESSAGE(STATUS "目标系统:${CMAKE_HOST_SYSTEM_NAME}")

INCLUDE(cmake-env/version.cmake)

INCLUDE(cmake-env/complie.cmake)

#是否使用SDK提供日志，如果使用需要在项目中链接 log4cplus库
option(USE_SDK_LOG4CPLUS "use sdk log4cplus" ON)
IF(USE_SDK_LOG4CPLUS)
    add_definitions(-DUSE_SDKLOG=1)
    MESSAGE(STATUS "使用log4cplus 为日志核心")
ELSE()
    MESSAGE(STATUS "使用控制台输出做日志核心")
ENDIF()


if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
	INCLUDE(cmake-env/linux-x86_64-env.cmake)
ELSEIF(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    INCLUDE(cmake-env/mac-env.cmake)
ELSEIF(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    MESSAGE(STATUS "暂时不支持Window")
ELSE()
    MESSAGE(STATUS "未知系统:${CMAKE_HOST_SYSTEM_NAME}")
ENDIF()

MESSAGE(STATUS "ALL_INCLUDE_DIRS:${ALL_INCLUDE_DIRS}")
MESSAGE(STATUS "ALL_LIBRARY_DIRS:${ALL_LIBRARY_DIRS}")
MESSAGE(STATUS "ALL_LIBRARIES:${ALL_LIBRARIES}")

#插件编译(主要都是用来测试的插件)
ADD_SUBDIRECTORY(testmain)

# ADD_SUBDIRECTORY(testlib1)

# ADD_SUBDIRECTORY(testlib2)