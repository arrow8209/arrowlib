cmake_minimum_required(VERSION 3.0)


MESSAGE(STATUS "*use linux x64")

#设置预定宏（按需修改）
#add_definitions(-DATLAS500)

#编译环境和参数设置（按需修改）
set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/x64/debug)
ELSE(${CMAKE_BUILD_TYPE})
    set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/x64/release)
endif()




set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_ROOT}/include)
set(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_ROOT}/lib)
set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_ROOT}/lib)

#设置编译参数（基本不改）
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -Wno-deprecated  -fpermissive -Wunknown-pragmas")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -Wno-deprecated  -fpermissive -Wunknown-pragmas")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic -Wunknown-pragmas")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic -Wunknown-pragmas")

set(ALL_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
set(ALL_LIBRARY_DIRS ${LOG4CPLUS_LIBRARY_DIRS})
