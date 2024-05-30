function(get_tareget_platform result_target_platform)

    message(STATUS "CMAKE_CXX_COMPILER:${CMAKE_CXX_COMPILER}")
    #针对编译器路径判读是否为aarch64编译器
    string(FIND "${CMAKE_CXX_COMPILER}" "aarch64" is_aarch64)
    IF(${is_aarch64} STREQUAL "-1")
        string(FIND "${CMAKE_C_COMPILER}" "aarch64" is_aarch64)
    ENDIF()

    #针对编译器判断是否为arm编译器
    string(FIND "${CMAKE_CXX_COMPILER}" "arm" is_arm)
    IF(${is_aarch64} STREQUAL "-1")
        string(FIND "${CMAKE_C_COMPILER}" "arm" is_arm)
    ENDIF()


    #设置编译环境变量
    IF(NOT ${is_aarch64} STREQUAL "-1")
        SET(${result_target_platform} "aarch64" PARENT_SCOPE)
    ELSEIF(NOT ${is_arm} STREQUAL "-1")
        SET(${result_target_platform} "arm" PARENT_SCOPE)
    ELSE()
        SET(${result_target_platform} "x86_64" PARENT_SCOPE)
    ENDIF()

    message(STATUS "result_target_platform:${result_target_platform}")
endfunction()
