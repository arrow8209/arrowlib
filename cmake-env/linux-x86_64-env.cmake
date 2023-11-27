cmake_minimum_required(VERSION 3.5)

#设置PkgConfig （无需修改）
FIND_PACKAGE(PkgConfig)

#设置第三方库信息
SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/log4cplus/build/2.1.0/x86_64-gcc9.5.0_glibc2.27_linux4.15.18/release/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
PKG_SEARCH_MODULE(log4cplus REQUIRED log4cplus)

#设置boost
set(boost_root /maxvision/lib/boost/build/1.82.0/x86_64-gcc9.5.0_glibc2.27_linux4.15.18/release)

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
