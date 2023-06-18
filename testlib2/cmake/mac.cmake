cmake_minimum_required(VERSION 3.0.0)
PROJECT (${ProjectName})

MESSAGE("* ${ProjectName} tagert: mac")

##编译环境和参数设置

#编译环境和参数设置（按需修改）
# set(CMAKE_CXX_STANDARD 11)

#设置预定宏（按需修改）
#add_definitions(-DATLAS500)


#编译前执行相关指令（按需修改）
EXEC_PROGRAM(cp ${CMAKE_CURRENT_SOURCE_DIR} ARGS "${CMAKE_CURRENT_SOURCE_DIR}/*.ini ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")


#工程添加多个特定的头文件搜索路径（按需修改）
INCLUDE_DIRECTORIES(
    ${ALL_INCLUDE_DIRS}
)

#添加非标准的共享库搜索路径（按需修改）
LINK_DIRECTORIES(
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${ALL_LIBRARY_DIRS}
)

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

ADD_LIBRARY(
	${ProjectName}
    SHARED
	${SRC_LIST} 
)

# ADD_EXECUTABLE(
#     ${ProjectName}
#     ${SRC_LIST} 
# )


TARGET_LINK_LIBRARIES(
	${ProjectName}
    log4cplus
    dl
    pthread
    # ${log4cplus_LIBRARIES}
)
