cmake_minimum_required(VERSION 3.0)


MESSAGE("* " "use linux x64")

#设置预定宏（按需修改）
#add_definitions(-DATLAS500)

#编译环境和参数设置（按需修改）
set(CMAKE_CXX_STANDARD 11)

INCLUDE(./cmake-env/x64-env/x64-com-dir.cmake)

MESSAGE("* " "LOG4CPLUS_ROOT:${LOG4CPLUS_ROOT}")

set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_ROOT}/include)
set(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_ROOT}/lib)
set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_ROOT}/lib)

#设置编译参数（基本不改）
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic -Wunknown-pragmas")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic -Wunknown-pragmas")

set(ALL_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
set(ALL_LIBRARY_DIRS ${LOG4CPLUS_LIBRARY_DIRS})
