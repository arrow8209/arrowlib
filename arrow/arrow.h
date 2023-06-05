#pragma once

// 辅助模块 [zhuyb 2022-12-15 10:13:23]
#include "other/load_lib.h"
#include "other/obj_pool_simple.h"
#include "other/out_run_time.h"
#include "other/std_assist.h"
#include "other/static_const.h"
#include "other/convert.h"

// pattern模块 [zhuyb 2022-12-15 10:13:29]
#include "pattern/singleton/singleton.h"
#include "pattern/singleton/singleton_unmanager.h"
#include "pattern/observer/observer.h"
#include "pattern/observer/subject.h"
#include "pattern/factory/factory.h"
#include "pattern/factory/static_factory.h"

// task模块 [zhuyb 2022-12-15 10:13:38]
#include "task/task_one_thread.h"

// 模板 [zhuyb 2022-12-15 10:13:46]
#include "typelist/typelist_type.h"
#include "typelist/typelist_fun.h"
#include "typelist/static_string.h"
#include "typelist/static_map.h"

#if __cplusplus >= 201402L
	#include "typelist/enum_to_str_14.h"
#elif __cplusplus >= 201103L
    #include "typelist/enum_to_str.h"
#endif
