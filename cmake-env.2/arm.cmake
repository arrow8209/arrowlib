# 初始版本，无法正常使用，仅供参考
cmake_minimum_required(VERSION 3.5)

MESSAGE(STATUS "include arm.cmake")

#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/)

# IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/debug)
#     set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/debug)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/debug)
# ELSE(${CMAKE_BUILD_TYPE})
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/release)
#     set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/release)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arm/release)
# ENDIF()


#设置PkgConfig 
FIND_PACKAGE(PkgConfig)

#设置gstreamer pkgconfig path
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/gstreamer/build/1.22.5/arm-eabi-gcc9.5.0_glibc2.27_linux4.15.18/debug/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
ELSE(${CMAKE_BUILD_TYPE})
    SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/gstreamer/build/1.22.5/arm-eabi-gcc9.5.0_glibc2.27_linux4.15.18/release/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
ENDIF()

PKG_SEARCH_MODULE(gstreamer-1.0 REQUIRED gstreamer-1.0)
PKG_SEARCH_MODULE(gstreamer-base-1.0 REQUIRED gstreamer-base-1.0)
PKG_SEARCH_MODULE(gstreamer-plugins-base-1.0 REQUIRED gstreamer-plugins-base-1.0)
PKG_SEARCH_MODULE(gstreamer-video-1.0 REQUIRED gstreamer-video-1.0)
PKG_SEARCH_MODULE(gmodule-2.0 REQUIRED gmodule-2.0)
PKG_SEARCH_MODULE(gobject-2.0 REQUIRED gobject-2.0)
PKG_SEARCH_MODULE(ffi REQUIRED libffi)
PKG_SEARCH_MODULE(pcre2-8 REQUIRED libpcre2-8)
PKG_SEARCH_MODULE(freetype2 REQUIRED freetype2)
PKG_SEARCH_MODULE(png REQUIRED libpng)

#设置boost
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(boost_root /maxvision/lib/boost/build/1.82.0/arm-eabi-gcc9.5.0_glibc2.27_linux4.15.18/debug)
ELSE(${CMAKE_BUILD_TYPE})
    set(boost_root /maxvision/lib/boost/build/1.82.0/arm-eabi-gcc9.5.0_glibc2.27_linux4.15.18/release)
ENDIF()

#设置 log4cplus
SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/log4cplus/build/2.1.0/arm-eabi-gcc9.5.0_glibc2.27_linux4.15.18/release/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
PKG_SEARCH_MODULE(log4cplus REQUIRED log4cplus)

# #设置 freetype2
# SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/freetype/build/2.13.0/arm-eabi-gcc9.5.0_glibc2.27_linux4.15.18/release/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
# PKG_SEARCH_MODULE(freetype2 REQUIRED freetype2)

#设置OpenCV包路径
set(OpenCV_DIR /maxvision/lib/opencv/build/4.8/arm-eabi-gcc9.5.0_glibc2.27_linux4.15.18/release/lib/cmake/opencv4)
find_package(OpenCV REQUIRED NO_CMAKE_FIND_ROOT_PATH)

#设置mott
# set(mott_root /maxvision/lib/mott/build/arm)

#设置eigen3
SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/eigen/share/pkgconfig:$ENV{PKG_CONFIG_PATH})
PKG_SEARCH_MODULE(eigen3 REQUIRED eigen3)

#设置头文件包含目录
SET(
    ALL_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}
    ${gstreamer-1.0_INCLUDE_DIRS}
    ${gstreamer-base-1.0_INCLUDE_DIRS}
    ${gstreamer-plugins-base-1.0_INCLUDE_DIRS}
    ${gstreamer-video-1.0_INCLUDE_DIRS}
    ${gmodule-2.0_INCLUDE_DIRS}
    ${gobject-2.0_INCLUDE_DIRS}
    ${ffi_INCLUDE_DIRS}
    ${pcre2-8_INCLUDE_DIRS}
    ${freetype2_INCLUDE_DIRS}
    ${png_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${log4cplus_INCLUDE_DIRS}
    ${eigen3_INCLUDE_DIRS}
    ${mott_root}/include
    ${boost_root}/include
)

#设置库目录
SET(
    ALL_LIBRARY_DIRS
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${gstreamer-1.0_LIBRARY_DIRS}
    ${gstreamer-base-1.0_LIBRARY_DIRS}
    ${gstreamer-plugins-base-1.0_LIBRARY_DIRS}
    ${gstreamer-video-1.0_LIBRARY_DIRS}
    ${gmodule-2.0_LIBRARY_DIRS}
    ${gobject-2.0_LIBRARY_DIRS}
    ${ffi_LIBRARY_DIRS}
    ${pcre2-8_LIBRARY_DIRS}
    ${freetype2_LIBRARY_DIRS}
    ${png_LIBRARY_DIRS}
    ${OpenCV_LIBRARY_DIRS}
    ${log4cplus_LIBRARY_DIRS}
    ${mott_root}/lib
    ${boost_root}/lib
)
#设置链接库
SET(
    ALL_LIBRARIES
    ${gstreamer-1.0_LIBRARIES}
    ${gstreamer-base-1.0_LIBRARIES}
    ${gstreamer-plugins-base-1.0_LIBRARIES}
    ${gstreamer-video-1.0_LIBRARIES}
    ${gmodule-2.0_LIBRARIES}
    ${gobject-2.0_LIBRARIES}
    ${ffi_LIBRARIES}
    ${pcre2-8_LIBRARIES}
    ${freetype2_LIBRARIES}
    ${png_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${log4cplus_LIBRARIES}
    pthread 
    stdc++
    libmott-static.a
    libm.so
    libc.so
    dl
    boost_system
    boost_filesystem
    boost_atomic
)

# MESSAGE(STATUS "ALL_INCLUDE_DIRS:${ALL_INCLUDE_DIRS}")
# MESSAGE(STATUS "ALL_LIBRARY_DIRS:${ALL_LIBRARY_DIRS}")
# MESSAGE(STATUS "ALL_LIBRARIES:${ALL_LIBRARIES}")
