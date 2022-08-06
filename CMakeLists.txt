cmake_minimum_required(VERSION 3.0)

#设置编译参数和环境
# SET(COMPILE_TYPE "linux_x64")
SET(COMPILE_TYPE "mac")


IF(${COMPILE_TYPE} STREQUAL "linux_x64")
	INCLUDE(cmake/linux_x64.cmake)
ELSEIF(${COMPILE_TYPE} STREQUAL "mac")
	INCLUDE(cmake/mac.cmake)
ELSE(${COMPILE_TYPE})
    MESSAGE("    *COMPILE_TYPE:  Unkonw ""${COMPILE_TYPE}")
    INCLUDE(cmake/Linux_x64.cmake)
ENDIF()