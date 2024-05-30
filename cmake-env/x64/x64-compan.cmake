
# MESSAGE(STATUS "${CMAKE_CURRENT_LIST_FILE}")

# #设置输出目录
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/)

# #添加boost库
# #按需修改
# SET(ADD_BOOST_LIBS "filesystem" "atomic")
# IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     SET(BOOST_ROOT "/mnt/data/third-lib/boost/build/1.82.0/x86_64-gcc9.5.0_glibc2.27_linux4.15.18/debug")
# ELSE(${CMAKE_BUILD_TYPE})
#     SET(BOOST_ROOT "/mnt/data/third-lib/boost/build/1.82.0/x86_64-gcc9.5.0_glibc2.27_linux4.15.18/release")
# ENDIF()

# #基本不用修改
# find_package(Boost 1.82.0 REQUIRED COMPONENTS ${ADD_BOOST_LIBS} HINTS "${BOOST_ROOT}/release")
# if(Boost_FOUND)
#     message(STATUS "Boost success")
#     message(STATUS "Boost_INCLUDE_DIR:${Boost_INCLUDE_DIR}")
#     message(STATUS "Boost_LIBRARY_DIRS:${Boost_LIBRARY_DIRS}")
#     message(STATUS "Boost_LIBRARIES:${Boost_LIBRARIES}")
# else()
#     message(FATAL_ERROR "Boost not found")
# endif()

# #添加log4cplus库
# #按需修改
# IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     SET(LOG4CPLUS_ROOT "/mnt/data/third-lib/log4cplus/build/2.1.0/x86_64-gcc9.5.0_glibc2.27_linux4.15.18/debug")
# ELSE(${CMAKE_BUILD_TYPE})
#     SET(LOG4CPLUS_ROOT "/mnt/data/third-lib/log4cplus/build/2.1.0/x86_64-gcc9.5.0_glibc2.27_linux4.15.18/release")
# ENDIF()
# #无需修改
# SET(LOG4CPLUS_INCLUDE_DIR ${LOG4CPLUS_ROOT}/include)
# SET(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_ROOT}/lib)
# SET(LOG4CPLUS_LIBRARIES log4cplus)

# #设置预定宏（按需修改）
# add_definitions(-DAMD64)

# #设置头文件包含目录
# SET(
#     ALL_INCLUDE_DIRS
#     ${CMAKE_SOURCE_DIR}
#     ${LOG4CPLUS_INCLUDE_DIR}
# )

# #设置库目录
# SET(
#     ALL_LIBRARY_DIRS
#     ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#     ${LOG4CPLUS_LIBRARY_DIRS}
# )

# #设置链接库
# SET(
#     ALL_LIBRARIES
#     pthread stdc++ m c dl
#     ${Boost_LIBRARIES}
#     ${LOG4CPLUS_LIBRARIES}
# )

# MESSAGE(STATUS "ALL_INCLUDE_DIRS:${ALL_INCLUDE_DIRS}")
# MESSAGE(STATUS "ALL_LIBRARY_DIRS:${ALL_LIBRARY_DIRS}")
# MESSAGE(STATUS "ALL_LIBRARIES:${ALL_LIBRARIES}")