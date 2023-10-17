cmake_minimum_required(VERSION 3.0)


MESSAGE("* " "use linux x64")

# if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     set(LOG4CPLUS_ROOT /maxvision/lib/log4cplus/build/amd64/debug)
# ELSE(${CMAKE_BUILD_TYPE})
#     set(LOG4CPLUS_ROOT /maxvision/lib/log4cplus/build/amd64/release)
# endif()

SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/log4cplus/build/2.1.0/amd64/release/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
PKG_SEARCH_MODULE(log4cplus REQUIRED log4cplus)