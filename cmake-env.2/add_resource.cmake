#将文件内容嵌入到程序中(初始用途为将模型文件嵌入到程序中)

#初始化
function(init_resource)
    set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resource/)
    file(MAKE_DIRECTORY ${RESOURCE_DIR})
    file(MAKE_DIRECTORY ${RESOURCE_DIR}/inl)
    file(MAKE_DIRECTORY ${RESOURCE_DIR}/src)
    file(WRITE ${RESOURCE_DIR}/res.h "")

    set(RESOURCE_DIR ${RESOURCE_DIR} PARENT_SCOPE)
    set(RESOURCE_FILES "" PARENT_SCOPE)
endfunction(init_resource)

function(add_resource file_path)
    # #获取文件名（包括后缀名）
    get_filename_component(file_name ${file_path} NAME)
    # 获取文件长度
    file(SIZE ${file_path} file_size) 
    
    set(file_header "${RESOURCE_DIR}/inl/${file_name}.h")
    set(file_cpp        "${RESOURCE_DIR}/src/${file_name}.c")
    
    # 判断是否传入了第三个参数
    if(ARGC GREATER 1)
        # 如果传入了第三个参数，则使用该参数
        set(token ${ARGV1})
    else(ARGC)
        # 将文件名转换为C风格字符串
        string(MAKE_C_IDENTIFIER ${file_name} token)
    endif()

    # 将c风格字符串转换为大写
    string(TOUPPER ${token} token)

    # 清空输出文件
    file(WRITE ${file_header} "")
    file(WRITE ${file_cpp} "")

    # 将资源文件转换为字节流
    file(READ ${file_path} filedata HEX)
    # 将字节流转为16进制字符串
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
    # 添加代码
    file(APPEND ${file_cpp}
        "const unsigned char ${token}_DATA[] = {\n${filedata}\n};\n"
        "const unsigned long ${token}_SIZE   = ${file_size};\n")
    file(APPEND ${file_header}
        "extern const unsigned char ${token}_DATA[];\n"
        "extern const unsigned long ${token}_SIZE;\n")
    # 加入到生成文件列表
    list(APPEND GEN_EMBED_FILES
        ${file_cpp}
        ${file_header}
    )

    file(APPEND ${RESOURCE_DIR}/res.h
        "#include \"${file_header}\"\n"
    )

    set(
        RESOURCE_FILES
        ${RESOURCE_FILES}
        ${file_cpp}
    )

    set(RESOURCE_FILES ${RESOURCE_FILES} PARENT_SCOPE)
endfunction()

function(end_resource)
    INCLUDE_DIRECTORIES(
        set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resource/)
    )
endfunction(end_resource)

