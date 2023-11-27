cmake_minimum_required(VERSION 3.5)

SET(ProjectName testmain)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
	INCLUDE(cmake/x64.cmake)
ELSEIF(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
	INCLUDE(cmake/mac.cmake)
ELSE()
    MESSAGE(STATUS "Unkonw HOST SYSTEM:${CMAKE_HOST_SYSTEM_NAME}")
ENDIF()
