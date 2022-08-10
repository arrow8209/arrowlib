cmake_minimum_required(VERSION 3.0)

#设置编译参数和环境
# SET(COMPILE_PLATFROM "linux_x64")
# SET(COMPILE_PLATFROM "mac")

SET(ProjectName arrow_lib_test)
IF(${COMPILE_PLATFROM} STREQUAL "x64")
	INCLUDE(cmake/x64.cmake)
ELSEIF(${COMPILE_PLATFROM} STREQUAL "mac")
	INCLUDE(cmake/mac.cmake)
ELSE(${COMPILE_PLATFROM})
    MESSAGE("* COMPILE_PLATFROM:  Unkonw ""${COMPILE_PLATFROM}")
    INCLUDE(cmake/x64.cmake)
ENDIF()