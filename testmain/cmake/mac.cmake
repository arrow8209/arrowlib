cmake_minimum_required(VERSION 3.0.0)
PROJECT (${ProjectName})

MESSAGE("* ${ProjectName} tagert: mac")

##编译环境和参数设置

#编译环境和参数设置（按需修改）
set(CMAKE_CXX_STANDARD 11)

#设置预定宏（按需修改）
#add_definitions(-DATLAS500)


#编译前执行相关指令（按需修改）

# EXEC_PROGRAM(cp ${CMAKE_CURRENT_SOURCE_DIR} ARGS "${CMAKE_CURRENT_SOURCE_DIR}/../algorithm/zhihuijiayouzhan/lib/lib/libmaxvision_algorithm_sdk_aarch64.so ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
# EXEC_PROGRAM(cp ${CMAKE_CURRENT_SOURCE_DIR} ARGS "-R ${CMAKE_CURRENT_SOURCE_DIR}/*.ini ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
# EXEC_PROGRAM(cp ${CMAKE_CURRENT_SOURCE_DIR} ARGS "-R ${CMAKE_CURRENT_SOURCE_DIR}/*.txt ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
# EXEC_PROGRAM(mkdir ${CMAKE_CURRENT_SOURCE_DIR} ARGS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/model")
# EXEC_PROGRAM(${CMAKE_CURRENT_SOURCE_DIR}/config/copymodeltobin.sh 
# ${CMAKE_CURRENT_SOURCE_DIR} ARGS 
# "${CMAKE_CURRENT_SOURCE_DIR}/../algorithm/zhihuijiayouzhan/model ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/model")


# message(STATUS "-R ${CMAKE_CURRENT_SOURCE_DIR}/../algorithm/zhihuijiayouzhan/model ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")


#设置编译参数（基本不改）
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall -rdynamic -Wno-deprecated  -fpermissive -Wunused-local-typedef")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wall -rdynamic -Wno-deprecated  -fpermissive -Wunused-local-typedef")
# # -stdlib=libstdc++
# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -O0 -g -Wall -pipe -Wextra -latomic -Wunused-local-typedef")
# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC -O2 -Wall -pipe -Wextra -latomic -Wunused-local-typedef")

#设置PkgConfig （无需修改）
# FIND_PACKAGE(PkgConfig)

#设置pkgconfig path（按需修改）
# SET(ENV{PKG_CONFIG_PATH} /maxvision/lib/gstreamer/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
# PKG_SEARCH_MODULE(gst-main REQUIRED gstreamer-1.0)

#设置 package 包路径
# set(log4cplus_DIR /Users/zhuyuanbo/Documents/5.code/third_party_libr/lib/log4cplus/x64/lib/cmake/log4cplus )
# find_package(log4cplus REQUIRED NO_CMAKE_FIND_ROOT_PATH)

#工程添加多个特定的头文件搜索路径（按需修改）
INCLUDE_DIRECTORIES(
    ${ALL_INCLUDE_DIRS}
)

#添加非标准的共享库搜索路径（按需修改）
LINK_DIRECTORIES(
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${ALL_LIBRARY_DIRS}
)

#需要编译的文件相关设置

#将文件添加到工程目录中（按需修改）
FILE(GLOB_RECURSE SRC_LIST *.cpp *.c *.h *.hpp)

#设置需要忽略的源文件（按需修改）
SET(IGNORE_SRC_LIST )

#从源文件列表中查找需要忽略文件（无需修改）
SET(SRC_REMOVE_LIST)
FOREACH(F ${SRC_LIST})
	FOREACH(I ${IGNORE_SRC_LIST})
		SET(RegexStr .*/${I})
		string(REGEX MATCH ${RegexStr} dirName ${F} )
		IF(dirName)
			LIST(APPEND SRC_REMOVE_LIST ${F})
		ENDIF(dirName)
	ENDFOREACH(I)
ENDFOREACH(F)

#从源文件中删除忽略文件（无需修改）
FOREACH(F ${SRC_REMOVE_LIST})
	list(FIND SRC_LIST ${F} TMP_INDEX)
	list(REMOVE_AT SRC_LIST ${TMP_INDEX} )
ENDFOREACH(F)

MESSAGE("==============Src List==============")
FOREACH(F ${SRC_LIST})
	MESSAGE("* ${F}")
ENDFOREACH(F)
MESSAGE("==============Src List==============")

# ADD_LIBRARY(
# 	${ProjectName}
#     SHARED
# 	${SRC_LIST} 
# )

ADD_EXECUTABLE(
    ${ProjectName}
    ${SRC_LIST} 
)


TARGET_LINK_LIBRARIES(
	${ProjectName}
    # log4cplus
    dl
    pthread
)
