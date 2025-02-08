# 检查是否已安装Conan
#
#说明
#host主机需要安装conan环境
#
#   apt install python3 python3-pip
#   pip3 install --upgrade pip
#   pip3 install conan==1.62.0
#   conan --version
#
#   export CONAN_USER_HOME=/maxvision/conan_cache
#
#如果host主机为容器,通过设置 export CONAN_USER_HOME=/path/to/custom/conan/cache 环境变量来指定缓存路径set(ENV{CONAN_USER_HOME} /maxvision/conan_cache)

# set(ENV{CONAN_USER_HOME} /maxvision/conan_cache)

function(arrow_check_conan_version)
    find_program(CONAN_EXE conan)

    if(NOT CONAN_EXE)
        message(FATAL_ERROR "未安装conan。请安装conan。")
    endif()

    # 检查版本是否为1.x
    execute_process(
        COMMAND conan --version
        RESULT_VARIABLE CONAN_VERSION_RESULT
        OUTPUT_VARIABLE CONAN_VERSION_OUTPUT
    )
    string(REPLACE "\n" "" CONAN_VERSION_OUTPUT "${CONAN_VERSION_OUTPUT}")

    if(NOT CONAN_VERSION_RESULT EQUAL 0)
        message(FATAL_ERROR "无法获取conan版本信息。")
    endif()

    string(REGEX MATCH "([0-9]+)\\.([0-9]+)" CONAN_VERSION_MATCH ${CONAN_VERSION_OUTPUT})

    if(NOT CMAKE_MATCH_1 EQUAL 1)
        message(STATUS "Conan version: ${CONAN_VERSION_OUTPUT}")
        message(FATAL_ERROR "不支持 ${CONAN_VERSION_OUTPUT} 建议使用1.62.0")
    else()
        message(STATUS "conan version:${CONAN_VERSION_OUTPUT}")
    endif()
endfunction()

function(check_conan_evn remote_addr conan_user conan_pwd)

    # 检测conan版本
    arrow_check_conan_version()

    string(REGEX MATCH "([^/]+)$" MATCHED ${remote_addr})
    set(remote_name ${CMAKE_MATCH_1})
    message(STATUS "remote_addr:${remote_addr}")
    message(STATUS "remote_name:${remote_name}")
    message(STATUS "conan_user:${conan_user}")
    message(STATUS "conan_pwd:${conan_pwd}")

    # 检查remote 是否add
    execute_process(
        COMMAND conan remote list
        RESULT_VARIABLE CONAN_REMOTE_LIST_RESULT
        OUTPUT_VARIABLE CONAN_REMOTE_LIST_OUTPUT
    )

    if(NOT CONAN_REMOTE_LIST_OUTPUT MATCHES ${remote_addr})
        execute_process(
            COMMAND conan remote add ${remote_name} ${remote_addr}
            RESULT_VARIABLE CONAN_REMOTE_ADD_RESULT
            OUTPUT_VARIABLE CONAN_REMOTE_ADD_OUTPUT
        )

        execute_process(
            COMMAND conan user -p ${conan_pwd} -r ${remote_name} ${conan_user}
            RESULT_VARIABLE CONAN_REMOTE_USER_RESULT
            OUTPUT_VARIABLE CONAN_REMOTE_USER_OUTPUT
        )
    endif()

    # 检查remote 是否add
    execute_process(
        COMMAND conan remote list
        RESULT_VARIABLE CONAN_REMOTE_LIST_RESULT
        OUTPUT_VARIABLE CONAN_REMOTE_LIST_OUTPUT
    )

endfunction()