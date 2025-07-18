cmake_minimum_required(VERSION 3.10)
project(test_enum2)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(test_constexpr test_constexpr.cpp)

# 设置包含目录
target_include_directories(test_constexpr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# # 输出目录
# set_target_properties(test_constexpr PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
# ) 

# 添加可执行文件
add_executable(test_enum_to_string main.cpp)

# 设置包含目录
target_include_directories(test_enum_to_string PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})