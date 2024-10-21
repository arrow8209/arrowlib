#版本号文件
find_package(Git)

# 生成版本描述字符串 对应git 的hash
execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --abbrev=8 --exclude=* --always --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "git version:${GIT_VERSION}")

# 获取 branch
execute_process(
    COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH_TMP
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "git branch:" ${GIT_BRANCH_TMP})

# 获取 tag
execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --exact-match HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_TAG_TMP
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "git tag:" ${GIT_TAG_TMP})

# 获取 index
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_INDEX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "git index:" ${GIT_INDEX}) 

IF(NOT GIT_TAG_TMP)
    SET(GIT_BRANCH ${GIT_BRANCH_TMP})
ELSE()
    SET(GIT_BRANCH ${GIT_TAG_TMP})
ENDIF()


#获取编译时间
string(TIMESTAMP COMPILE_TIME %Y%m%d)
string(SUBSTRING ${COMPILE_TIME} 2 6 COMPILE_TIME)
set(BUILD_TIME    ${COMPILE_TIME})
message(STATUS "build time :${BUILD_TIME}")

# 写入头文件宏定义的版本字符串，日期和 Hash
file(WRITE version.h
"#pragma once
#define GIT_VERSION \"${GIT_VERSION}\"
#define GIT_BRANCH \"${GIT_BRANCH}\"
#define GIT_INDERX_STR \"${GIT_INDEX}\"
#define GIT_INDERX_VAL ${GIT_INDEX}
#define BUILD_TIME \"${BUILD_TIME}\"
#define VERSION \"${BUILD_TIME} ${GIT_BRANCH} ${GIT_VERSION} \""
)
