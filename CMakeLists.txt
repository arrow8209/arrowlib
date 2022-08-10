cmake_minimum_required(VERSION 3.0)

PROJECT(arrowlib)

#设置编译参数和环境
SET(COMPILE_PLATFROM "x64")
# SET(COMPILE_PLATFROM "mac")




IF(${COMPILE_PLATFROM} STREQUAL "x64")
	INCLUDE(cmake-env/x64-env.cmake)
    SET(PlatFromName x64)
ELSEIF(${COMPILE_PLATFROM} STREQUAL "mac")
    INCLUDE(cmake-env/mac-env.cmake)
    SET(PlatFromName mac)
ELSE(${COMPILE_PLATFROM})
    MESSAGE("* COMPILE_PLATFROM:  Unkonw ""${COMPILE_PLATFROM}")
    INCLUDE(cmake-env/x64-env.cmake)
    SET(PlatFromName x64)
ENDIF()


#插件编译(主要都是用来测试的插件)
ADD_SUBDIRECTORY(testmain)

ADD_SUBDIRECTORY(testlib1)