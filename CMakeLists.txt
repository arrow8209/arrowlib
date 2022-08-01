cmake_minimum_required(VERSION 3.0.0)
project(testpattern VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
add_executable(testpattern main.cpp)
TARGET_LINK_LIBRARIES(testpattern pthread stdc++)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
