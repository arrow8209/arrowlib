cmake_minimum_required(VERSION 3.14)

project(test_enum3)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(test_enum3_exec main.cpp)

# 添加包含目录
target_include_directories(test_enum3_exec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 