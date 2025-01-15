cmake_minimum_required(VERSION 3.5)

PROJECT (testmain)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fsanitize=address ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fsanitize=address ")

INCLUDE(cmake/default.cmake)