#pragma once
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <iostream>
#include "arrow/typelist/static_str/string_view.h"
#include "arrow/typelist/static_string.h"
#include "arrow/typelist/type_list_fun/type_to_data.h"
#include "arrow/typelist/type_name.h"


class COutFunName
{
public:
    static void Name1(int n1, int n2)
    {
        std::cout << __func__ << std::endl;
    }

    void Name2(float f1)
    {
        std::cout << __func__ << std::endl;
    }
};


static void TestFunName()
{
    constexpr auto func1 = Arrow::StaticStr::Str(__func__);
    using FunType = __ARROW_FUN_NAME_TYPE__;
    std::cout << func1.data << std::endl;
    std::cout << Arrow::ValueListToData<FunType>::data << std::endl;
    std::cout << __func__ << std::endl;
    COutFunName::Name1(0, 1);
    COutFunName c1;
    c1.Name2(2.0);
}
