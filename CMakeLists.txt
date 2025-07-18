cmake_minimum_required(VERSION 3.27)

PROJECT(arrowlib)

#设定编译参数
INCLUDE(cmake-env/complie.cmake)

#生成版本号文件
INCLUDE(cmake-env/version.cmake)

#获取目标平台
INCLUDE(cmake-env/get_target_complate.cmake)

#使用log4cplus库
add_definitions(-DUSE_SDKLOG=1)

IF(${target_platform} STREQUAL "x64")
    INCLUDE(cmake-env/x64-env.cmake)
ELSEIF(${target_platform} STREQUAL "aarch64")
    INCLUDE(cmake-env/aarch64-env.cmake)
ELSEIF(${target_platform} STREQUAL "arm")
    MESSAGE(STATUS "* COMPILE_PLATFROM:  Unkonw ${COMPILE_PLATFROM}")
ELSE(${target_platform})
    MESSAGE(STATUS "* COMPILE_PLATFROM:  Unkonw ${COMPILE_PLATFROM}")
ENDIF()

# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=address ")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address ")
# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=address ")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address ")

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O1 -fsanitize=address -fno-omit-frame-pointer")
# set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -fsanitize=address")
# set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -rdynamic")

# #插件编译(主要都是用来测试的插件)
# ADD_SUBDIRECTORY(test/testmain testmain)

# ADD_SUBDIRECTORY(test/testlib1 testlib1)

# ADD_SUBDIRECTORY(test/testlib2 testlib2)

ADD_SUBDIRECTORY(test/test_enum2 test_enum2)
ADD_SUBDIRECTORY(test/test_enum3 test_enum3)