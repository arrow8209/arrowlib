cmake_minimum_required(VERSION 3.0)

#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH            ${CMAKE_SOURCE_DIR}/bin)

set(GccCompailParam " -pipe -Wall -fPIC -rdynamic ")
set(GccCompailParam "${GccCompailParam} -Werror=return-type -Werror=format -Wno-unused-variable -Wno-deprecated ")
set(GccCompailParam "${GccCompailParam} -Wno-unused-function -Wno-reorder -Wno-unused-but-set-variable ")
set(GccCompailParam "${GccCompailParam} -Wno-unknown-pragmas -Wno-unused-result -Wno-maybe-uninitialized ")

# -pipe 使用管道替代编译的临时文件（可以加快编译速度）
# -fPIC 使用相对地址加载共享库
# -rdynamic 将所有链接的所有符号，添加到动态符号表
# -Werror=return-type 有返回值的函数,不写return 报错
# -Wno-unused-variable 未使用变量不报警
# -Wno-deprecated 使用过时的特性不要给出警告
# -Wno-unused-function 未使用函数不报警
# -Wno-reorder  编译器将代码重新排序时给出警告
# -Wno-unused-but-set-variable 仅设置的变量不给出警告
# -Wno-unknown-pragmas  对无法识别的 pragma 不给出警告
# -Wno-unused-result 不使用函数返回值不给出警告
# -Wno-maybe-uninitialized

#设置编译参数（基本不改）
# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g ${GccCompailParam}" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 ${GccCompailParam}")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g ${GccCompailParam}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 ${GccCompailParam}")



#编译环境和参数设置（按需修改）
set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedef")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedef ")

# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic -Wunknown-pragmas")
# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic -Wunknown-pragmas")

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-deprecated -Wno-unknown-pragmas")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-deprecated -Wno-unknown-pragmas")
# # -fsanitize=address 
# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic")
# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic")