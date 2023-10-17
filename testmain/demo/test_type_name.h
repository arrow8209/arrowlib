#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/typelist/type_name.h"
#include "arrow/typelist/type_name/enum_name_impl.h"

enum class TestEnum1
{
    Em1Item1 = 0,
    Em1Item2,
    Em1Item3,
    Em1ItemA = 10,
    Em1ItemB,
    Em1ItemC
};

enum TestEnum2
{
    Em2Item1 = 0,
    Em2Item2,
    Em2Item3,
    Em2ItemA = 10,
    Em2ItemB,
    Em2ItemC
};
namespace AiServer
{

enum EmErrorCode1
{
    Success = 0,
    E12345678
};

enum class EmErrorCode2
{
    Success = 0,
    E12345679
} ;

// typedef Arrow::EnumListToStr<EmErrorCode, 
//         Success, ErrorDecoderTemplateNoExist,
//         ErrorCann, ErrorCannResizeEndFail,
//         ErrorPipelineUnknowFail, ErrorTaskInfoParseFail,
//         ErrorDevInterfaceUnknowFail, ErrorDevInterfaceEnd> EmErrorCodeStr;
}

static void TestTypeName()
{
    using typeName1=Arrow::TypeName<std::map<int, std::string>>;
    typeName1::Trace();
    std::cout << typeName1::Name() << std::endl;

    std::map<int, std::string> tmp;
    std::cout << Arrow::GetTypeName(tmp) <<std::endl;
}

static void TestEnumName()
{
    using enumName1 = Arrow::EnumItemName<AiServer::EmErrorCode1, AiServer::EmErrorCode1::E12345678>;
    // enumName1::Trace1();
    // std::cout << enumName1::ImplName().data << std::endl;
    // std::cout << enumName1::ImplFullName().data << std::endl;
    std::cout << enumName1::ShortName() << std::endl;
    std::cout << enumName1::LongName() << std::endl;

    using enumName2 = Arrow::EnumItemName<AiServer::EmErrorCode2, AiServer::EmErrorCode2::E12345679>;
    // enumName2::Trace();
    // std::cout << enumName2::ImplName().data << std::endl;
    // std::cout << enumName2::ImplFullName().data << std::endl;
    std::cout << enumName2::ShortName() << std::endl;
    std::cout << enumName2::LongName() << std::endl;


    using enumName3 = Arrow::EnumItemName<TestEnum1, TestEnum1::Em1Item1>;
    // enumName3::Trace();
    // std::cout << enumName3::ImplName().data << std::endl;
    // std::cout << enumName3::ImplFullName().data << std::endl;
    std::cout << enumName3::ShortName() << std::endl;
    std::cout << enumName3::LongName() << std::endl;

    using enumName4 = Arrow::EnumItemName<TestEnum2, Em2Item1>;
    // enumName4::Trace();
    // std::cout << enumName3::ImplName().data << std::endl;
    // std::cout << enumName3::ImplFullName().data << std::endl;
    std::cout << enumName4::ShortName() << std::endl;
    std::cout << enumName4::LongName() << std::endl;

    // using enumName2 = Arrow::EnumItemName<AiServer::EmErrorCode2, AiServer::EmErrorCode2::E12345679>;
    // enumName2::Trace();
    // std::cout << enumName2::Name() << std::endl;
    // std::cout << enumName2::FullName() << std::endl;

    //     using enumName1 = Arrow::EnumItemName<AiServer::EmErrorCode, AiServer::EmErrorCode::ErrorDevInterfaceCreateEncoderH264Fail>;
    // enumName1::Trace();
    // std::cout << enumName1::ImplName().data << std::endl;
    // auto value = enumName1::ImplName();

    // std::cout << enumName1::ImplName().data << std::endl;

    // std::cout << enumName1::ImplFullName().data << std::endl;
    // std::cout << enumName1::Name() << std::endl;
    // std::cout << enumName1::FullName() << std::endl;

    // using enum1Name1 = Arrow::EnumItemName<TestEnum1, TestEnum1::Em1ItemA>;
    // enum1Name1::Trace();
    // std::cout << enum1Name1::Name() << std::endl;
    // std::cout << enum1Name1::FullName() << std::endl;

    // using enum2Name1 = Arrow::EnumItemName<TestEnum2, TestEnum2::Em2Item1>;
    // enum2Name1::Trace();
    // std::cout << enum2Name1::Name() << std::endl;
    // std::cout << enum2Name1::FullName() << std::endl;

    // using TestEnum1ListStr = Arrow::EnumListToStr<TestEnum1, TestEnum1::Em1Item1, TestEnum1::Em1ItemC>;
    // std::cout << TestEnum1ListStr::ItemStr(TestEnum1::Em1Item1) << std::endl;
    // std::cout << TestEnum1ListStr::ItemStr(TestEnum1::Em1ItemC) << std::endl;

    // using TestEnum1ListStr = Arrow::EnumListToStr<TestEnum1, Em1Item1, Em1Item3, Em1ItemA, Em1ItemC>;
    // std::cout << TestEnum1ListStr::ItemStr(Em1Item1) << std::endl;
    // std::cout << TestEnum1ListStr::ItemStr(Em1ItemC) << std::endl;
}