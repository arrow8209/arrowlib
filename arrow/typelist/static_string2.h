#pragma once

#include "static_str/string_view.h"
#include "static_str/find.h"
#include "static_str/sub.h"
#include "static_str/djb.h"
#include "static_str/file_name.h"

#ifdef WIN32
#define GET_FILE_NAME(fullFileName) Arrow::StaticStr::SubStr<Arrow::StaticStr::FindLast(fullFileName, '\\') + 1>(fullFileName)
#else
#define GET_FILE_NAME(fullFileName) Arrow::StaticStr::SubStr<Arrow::StaticStr::FindLast(fullFileName, '/') + 1>(fullFileName)
#endif

#define __ARROW_FILE__ GET_FILE_NAME(__FILE__)