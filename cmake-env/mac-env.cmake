cmake_minimum_required(VERSION 3.0)


MESSAGE(STATUS "*use mac-env")
#编译环境和参数设置（按需修改）
set(CMAKE_CXX_STANDARD 11)
set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/mac)

set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_ROOT}/include)
set(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_ROOT}/lib)
set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_ROOT}/lib)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}      -Wno-unused-function")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -Wall -Wextra -latomic -fPIC -fpermissive -Werror=return-type -Wno-unused-function -Wno-unused-variable -Wno-reorder -Wno-unused-but-set-variable -Wno-unused-function")



#设置编译参数（基本不改）
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -fpermissive -Werror=return-type -Wunused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -fpermissive -Werror=return-type -Wunused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic")

set(ALL_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
set(ALL_LIBRARY_DIRS ${LOG4CPLUS_LIBRARY_DIRS})