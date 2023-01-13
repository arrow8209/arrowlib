cmake_minimum_required(VERSION 3.0)


MESSAGE("* " "use linux x64")

#设置编译参数（基本不改）
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedefs")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -fpermissive -Werror=return-type -Wno-unused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedefs")

# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic -Wunknown-pragmas")
# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic -Wunknown-pragmas")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -Wno-deprecated -fpermissive -Werror=return-type -Wno-unused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedefs  -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -Wno-deprecated -fpermissive -Werror=return-type -Wno-unused-local-typedef -Wno-unused-but-set-variable  -Wno-reorder -Wno-deprecated -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedefs  -Wno-unknown-pragmas")
# -fsanitize=address 
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic -Wunknown-pragmas")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic -Wunknown-pragmas")


#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH            ${CMAKE_SOURCE_DIR}/bin/x64/${CMAKE_BUILD_TYPE})

#设置第三方库信息
INCLUDE(./cmake-env/x64-env/x64-com-dir.cmake)

set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_ROOT}/include)
set(LOG4CPLUS_LIBRARY_DIRS ${LOG4CPLUS_ROOT}/lib)
set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_ROOT}/lib)

set(ALL_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
set(ALL_LIBRARY_DIRS ${LOG4CPLUS_LIBRARY_DIRS} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

MESSAGE("* " "ALL_INCLUDE_DIRS:${ALL_INCLUDE_DIRS}")
MESSAGE("* " "ALL_LIBRARY_DIRS:${ALL_LIBRARY_DIRS}")
