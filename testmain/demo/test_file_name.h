#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/typelist/static_string2.h"
#include "arrow/typelist/type_name.h"


static void TestFileName()
{
    // constexpr auto fileName = Arrow::StaticStr::GetFileName(__FILE__);
    constexpr auto fileName =__ARROW_FILE__;
    // constexpr auto fileName = Arrow::StaticStr::SubStr<Arrow::StaticStr::FindLast(__FILE__, '/') + 1>(__FILE__);
    std::cout << fileName.data << std::endl;
}
