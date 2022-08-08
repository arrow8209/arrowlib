cmake_minimum_required(VERSION 3.0)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/x64/debug)
ELSE(${CMAKE_BUILD_TYPE})
    set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/x64/release)
endif()
