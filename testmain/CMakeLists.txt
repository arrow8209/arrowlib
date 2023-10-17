cmake_minimum_required(VERSION 3.5)

SET(ProjectName testmain)

IF(${COMPILE_PLATFROM} STREQUAL "x64")
	INCLUDE(cmake/x64.cmake)
ELSEIF(${COMPILE_PLATFROM} STREQUAL "mac")
	INCLUDE(cmake/mac.cmake)
ELSE(${COMPILE_PLATFROM})
    MESSAGE("* COMPILE_PLATFROM:  Unkonw ""${COMPILE_PLATFROM}")
ENDIF()
