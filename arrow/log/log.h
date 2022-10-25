/*
 * @FilePath: /arrowlib/arrow/log/log.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-07-07 18:26:24
 * @Description: 日志输出,使用log4cplus输出日志
 *
 *
 * SET(ENV{PKG_CONFIG_PATH} /home/zhuyb/Documents/2.lib/maxvision/lib/log4cplus/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
 * PKG_SEARCH_MODULE(log4cplus REQUIRED log4cplus)
 *
 */
#pragma once
#include "log_interface.h"
#include "log_impl_null.h"
#include "log_impl_std.h"
// #include "log_impl_log4cplus.h"

namespace Arrow
{

template<typename T>
using TLog=Log::base_log<T>;


} // namespace Arrow