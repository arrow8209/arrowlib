cmake_minimum_required(VERSION 3.0)


MESSAGE("* " "use linux x64")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(LOG4CPLUS_ROOT /maxvision/lib/log4cplus/build/amd64/debug)
ELSE(${CMAKE_BUILD_TYPE})
    set(LOG4CPLUS_ROOT /maxvision/lib/log4cplus/build/amd64/release)
endif()

