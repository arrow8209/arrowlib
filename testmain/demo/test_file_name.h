#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/typelist/static_string.h"
#include "arrow/typelist/type_name.h"

using FileNameType = __ARROW_FILE_NAME_TYPE__;
static void TestFileName()
{
    // constexpr auto fileName = Arrow::StaticStr::GetFileName(__FILE__);  
    // constexpr auto fileName = Arrow::StaticStr::SubStr<Arrow::StaticStr::FindLast(__FILE__, '/') + 1>(__FILE__);
    // constexpr const char* fileName = __ARROW_FILE_NAME_VALUE__;
    std::cout << __ARROW_FILE_NAME_VALUE__ << std::endl;
    std::cout << Arrow::ValueListToData<FileNameType>::data << std::endl;

}
