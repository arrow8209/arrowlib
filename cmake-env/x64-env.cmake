cmake_minimum_required(VERSION 3.0)


MESSAGE("* " "use linux x64")

#设置PkgConfig （无需修改）
FIND_PACKAGE(PkgConfig)

#设置第三方库信息
# INCLUDE(./cmake-env/x64-env/x64-com-dir.cmake)
SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/log4cplus/build/2.1.0/amd64/release/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
PKG_SEARCH_MODULE(log4cplus REQUIRED log4cplus)

#设置boost
set(boost_root /maxvision/lib/boost/build/amd64/release)

set(
    ALL_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}
    ${log4cplus_INCLUDE_DIRS} 
    ${boost_root}/include
)

set(
    ALL_LIBRARY_DIRS 
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    ${log4cplus_LIBRARY_DIRS} 
    ${boost_root}/lib
)
#设置链接库
SET(
    ALL_LIBRARIES
    ${log4cplus_LIBRARIES}
    pthread
)


MESSAGE(STATUS "ALL_INCLUDE_DIRS:${ALL_INCLUDE_DIRS}")
MESSAGE(STATUS "ALL_LIBRARY_DIRS:${ALL_LIBRARY_DIRS}")
MESSAGE(STATUS "ALL_LIBRARIES:${ALL_LIBRARIES}")