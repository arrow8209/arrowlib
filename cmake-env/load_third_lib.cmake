#根据目录参数获取第三方库的相关变量，并设置到全局变量中

function(load_boost root_dir)
    # 基本不用修改
    if(NOT CMAKE_BUILD_TYPE)
        SET(BOOST_ROOT "${root_dir}/release")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        SET(BOOST_ROOT "${root_dir}/debug")
    else()
        SET(BOOST_ROOT "${root_dir}/release")
    ENDIF()


    SET(Boost_INCLUDE_DIR ${BOOST_ROOT}/include)
    SET(Boost_LIBRARY_DIRS ${BOOST_ROOT}/lib)

    SET(Boost_INCLUDE_DIR ${Boost_INCLUDE_DIR} PARENT_SCOPE)
    SET(Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIRS} PARENT_SCOPE)

    SET(
        ALL_INCLUDE_DIRS
        ${ALL_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIR}
        PARENT_SCOPE
    )

    # 设置库目录
    SET(
        ALL_LIBRARY_DIRS
        ${ALL_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
        PARENT_SCOPE
    )

    # 设置链接库
    SET(
        ALL_LIBRARIES
        ${ALL_LIBRARIES}
        ${Boost_LIBRARIES}
        PARENT_SCOPE
    )
endfunction()


function(load_log4cplus root_dir)

    # # 基本不用修改
    # IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    #     SET(LOG4CPLUS_ROOT "${root_dir}/debug")
    # ELSE(${CMAKE_BUILD_TYPE})
    #     SET(LOG4CPLUS_ROOT "${root_dir}/release")
    # ENDIF()

    SET(LOG4CPLUS_ROOT "${root_dir}/release")

    SET(LOG4CPLUS_INCLUDE_DIR ${LOG4CPLUS_ROOT}/include)
    SET(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_ROOT}/lib)
    SET(LOG4CPLUS_LIBRARIES log4cplus)

    SET(LOG4CPLUS_INCLUDE_DIR ${LOG4CPLUS_INCLUDE_DIR} PARENT_SCOPE)
    SET(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_LIBRARY_DIRS} PARENT_SCOPE)
    SET(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_LIBRARIES} PARENT_SCOPE)

    SET(
        ALL_INCLUDE_DIRS
        ${ALL_INCLUDE_DIRS}
        ${LOG4CPLUS_INCLUDE_DIR}
        PARENT_SCOPE
    )

    # 设置库目录
    SET(
        ALL_LIBRARY_DIRS
        ${ALL_LIBRARY_DIRS}
        ${LOG4CPLUS_LIBRARY_DIRS}
        PARENT_SCOPE
    )

    # 设置链接库
    SET(
        ALL_LIBRARIES
        ${ALL_LIBRARIES}
        ${LOG4CPLUS_LIBRARIES}
        PARENT_SCOPE
    )
endfunction()