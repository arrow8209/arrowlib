cmake_minimum_required(VERSION 3.5)

MESSAGE(STATUS "include x64-env.cmake")

#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/)

#设置预定宏（按需修改）
add_definitions(-DAMD64)

#设置PkgConfig （无需修改）
FIND_PACKAGE(PkgConfig)

set(CONAN_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/../build_conan/x64/${CMAKE_BUILD_TYPE})
if(NOT EXISTS ${CONAN_OUT_DIR})
    check_conan_evn(http://172.31.50.215:8081/artifactory/api/conan/ai_server zhuyuanbo Zyb123456)
    check_conan_evn(http://172.31.50.215:8081/artifactory/api/conan/TensorRTLib zhuyuanbo Zyb123456)
    check_conan_evn(http://172.31.50.215:8081/artifactory/api/conan/ForwardSdk zhuyuanbo Zyb123456)
    conan_cmake_configure(REQUIRES
        gstreamer/1.22.5_glibc2.27@third-party/stable
        boost/1.82.0_glibc2.27@third-party/stable
        log4cplus/2.1.0_glibc2.27@third-party/stable
        opencv/4.8_glibc2.27@third-party/stable
        eigen/3.4.0@third-party/stable
        curl/8.11.0_glibc2.27@third-party/stable
        sasl/2.1.28_glibc2.27@third-party/stable
        libevent/2.1.12_glibc2.27@third-party/stable
        librdkafka/2.6.1_glibc2.27@third-party/stable
        openssl/1.1.1w_glibc2.27@third-party/stable
        psl/0.21.5_glibc2.27@third-party/stable
        lz4/1.10.0_glibc2.27@third-party/stable
        nghttp2/1.64.0_glibc2.27@third-party/stable
        zstd/1.5.6_glibc2.27@third-party/stable
        # openssl/1.1.1u@third-party/stable
        mott/ec0928e6@wyb/stable
        tensorrt_v8_x64/1.0.0@TensorrtLib_v8_x64/stable
        preprocessinferlibrary/277bf564@lzn/stable
        algorithm_lib/1.0.0@AlgorithmLib_TensorRT_V8.2.4/stable
        GENERATORS cmake)
        # GENERATORS CMakeDeps)
    conan_cmake_autodetect(settings)
    conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        # REMOTE ai_server
        INSTALL_FOLDER ${CONAN_OUT_DIR}
        SETTINGS "build_type=Release;arch=x86_64")
ENDIF()
include(${CONAN_OUT_DIR}/conanbuildinfo.cmake)

#设置头文件包含目录
SET(
    ALL_INCLUDE_DIRS
    ${ALL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CONAN_INCLUDE_DIRS}
)

#设置库目录
SET(
    ALL_LIBRARY_DIRS
    ${ALL_LIBRARY_DIRS}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${CONAN_LIB_DIRS}
)
# 设置链接库
SET(
    ALL_LIBRARIES
    ${ALL_LIBRARIES}
    ${CONAN_LIBS_GSTREAMER}
    ${CONAN_LIBS_BOOST}
    ${CONAN_LIBS_LOG4CPLUS}
    ${CONAN_LIBS_OPENCV}
    pthread 
    stdc++
    m c dl rt
)

# MESSAGE(STATUS "ALL_INCLUDE_DIRS:${ALL_INCLUDE_DIRS}")
# MESSAGE(STATUS "ALL_LIBRARY_DIRS:${ALL_LIBRARY_DIRS}")
# MESSAGE(STATUS "ALL_LIBRARIES:${ALL_LIBRARIES}")