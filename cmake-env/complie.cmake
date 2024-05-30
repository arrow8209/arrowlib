cmake_minimum_required(VERSION 3.5)

#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH            ${CMAKE_SOURCE_DIR}/bin)

#设置编译参数
set(GccCompailParam "${GccCompailParam} -Wall") # -Wall 开启所有警告
set(GccCompailParam "${GccCompailParam} -pipe") # -pipe 使用管道替代编译的临时文件（可以加快编译速度）
set(GccCompailParam "${GccCompailParam} -fPIC") # -fPIC 使用相对地址加载共享库
set(GccCompailParam "${GccCompailParam} -rdynamic") # -rdynamic 将所有链接的所有符号，添加到动态符号表
set(GccCompailParam "${GccCompailParam} -Werror=return-type") # -Werror=return-type 有返回值的函数,不写return 报错
set(GccCompailParam "${GccCompailParam} -Werror=format")
set(GccCompailParam "${GccCompailParam} -Wno-unused-variable")  # -Wno-unused-variable 未使用变量不报警
set(GccCompailParam "${GccCompailParam} -Wno-deprecated")   # -Wno-deprecated 使用过时的特性不要给出警告
set(GccCompailParam "${GccCompailParam} -Wno-unused-function")  # -Wno-unused-function 未使用函数不报警
set(GccCompailParam "${GccCompailParam} -Wno-reorder")  # -Wno-reorder  编译器将代码重新排序时给出警告
set(GccCompailParam "${GccCompailParam} -Wno-unused-but-set-variable")  # -Wno-unused-but-set-variable 仅设置的变量不给出警告
set(GccCompailParam "${GccCompailParam} -Wno-unknown-pragmas") # -Wno-unknown-pragmas  对无法识别的 pragma 不给出警告
set(GccCompailParam "${GccCompailParam} -Wno-unused-result")    # -Wno-unused-result 不使用函数返回值不给出警告
set(GccCompailParam "${GccCompailParam} -Wno-unused-local-typedefs")    # -Wno-unused-local-typedefs 不使用函数返回值不给出警告
set(GccCompailParam "${GccCompailParam} -Wno-uninitialized")   # -Wno-uninitialized 未初始化变量不报警
set(GccCompailParam "${GccCompailParam} -fvisibility=hidden")   # -fvisibility=hidden 默认隐藏所有接口
set(GccCompailParam "${GccCompailParam} -fstack-protector-all") # -fstack-protector-all 启用堆栈保护机制
set(GccCompailParam "${GccCompailParam} -fconstexpr-depth=2048") # -fconstexpr-depth 设置模板迭代深度
set(GccCompailParam "${GccCompailParam} -Wno-pmf-conversions") #取消对象函数指针转换警告

#设置编译参数（基本不改）
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${GccCompailParam} -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${GccCompailParam} -O0 -g" )

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ${GccCompailParam} -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${GccCompailParam} -O2 -DNDEBUG")
