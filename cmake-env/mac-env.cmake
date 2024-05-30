cmake_minimum_required(VERSION 3.5)

MESSAGE(STATUS "*use mac-env")

#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mac/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mac/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mac/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH            ${CMAKE_SOURCE_DIR}/bin/mac/${CMAKE_BUILD_TYPE})

#设置第三方库信息
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/x64/debug)
ELSE(${CMAKE_BUILD_TYPE})
    set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/x64/release)
endif()
#set(LOG4CPLUS_ROOT /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/mac)

set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_ROOT}/include)
set(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_ROOT}/lib)
set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_ROOT}/lib)

set(
    ALL_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}
    ${LOG4CPLUS_INCLUDE_DIRS} 
)

set(
    ALL_LIBRARY_DIRS 
    ${LOG4CPLUS_LIBRARY_DIRS} 
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

SET(
    ALL_LIBRARIES
    ${log4cplus_LIBRARIES}
    pthread
)
