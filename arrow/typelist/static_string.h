#pragma once
#include "typelist_type.h"
#include "typelist_fun.h"
#include "static_str/string_view.h"
#include "static_str/find.h"
#include "static_str/sub.h"
#include "static_str/djb.h"

// 最长支持1024个字符 [zhuyb 2022-07-30 20:25:54]
#define MakeCharSequence_16(m, n, str) Arrow::StaticStr::get<0x##m##n##0>(str), Arrow::StaticStr::get<0x##m##n##1>(str), \
                                       Arrow::StaticStr::get<0x##m##n##2>(str), Arrow::StaticStr::get<0x##m##n##3>(str), \
                                       Arrow::StaticStr::get<0x##m##n##4>(str), Arrow::StaticStr::get<0x##m##n##5>(str), \
                                       Arrow::StaticStr::get<0x##m##n##6>(str), Arrow::StaticStr::get<0x##m##n##7>(str), \
                                       Arrow::StaticStr::get<0x##m##n##8>(str), Arrow::StaticStr::get<0x##m##n##9>(str), \
                                       Arrow::StaticStr::get<0x##m##n##a>(str), Arrow::StaticStr::get<0x##m##n##b>(str), \
                                       Arrow::StaticStr::get<0x##m##n##c>(str), Arrow::StaticStr::get<0x##m##n##d>(str), \
                                       Arrow::StaticStr::get<0x##m##n##e>(str), Arrow::StaticStr::get<0x##m##n##f>(str)

#define MakeCharSequence_64(m, str) MakeCharSequence_16(m, 0, str), MakeCharSequence_16(m, 1, str), \
                                    MakeCharSequence_16(m, 2, str), MakeCharSequence_16(m, 3, str), \
                                    MakeCharSequence_16(m, 4, str), MakeCharSequence_16(m, 5, str), \
                                    MakeCharSequence_16(m, 6, str), MakeCharSequence_16(m, 7, str), \
                                    MakeCharSequence_16(m, 8, str), MakeCharSequence_16(m, 9, str), \
                                    MakeCharSequence_16(m, a, str), MakeCharSequence_16(m, b, str), \
                                    MakeCharSequence_16(m, c, str), MakeCharSequence_16(m, d, str), \
                                    MakeCharSequence_16(m, e, str), MakeCharSequence_16(m, f, str)

#define MakeCharSequence_128(str) MakeCharSequence_64(0, str), \
                                  MakeCharSequence_64(1, str)

#define MakeCharSequence_256(str) MakeCharSequence_64(0, str), \
                                  MakeCharSequence_64(1, str), \
                                  MakeCharSequence_64(2, str)

#define MakeCharSequence_512(str) MakeCharSequence_64(0, str), \
                                  MakeCharSequence_64(1, str), \
                                  MakeCharSequence_64(2, str), \
                                  MakeCharSequence_64(3, str)

#define MakeCharSequence_1024(str) MakeCharSequence_64(0, str), \
                                   MakeCharSequence_64(1, str), \
                                   MakeCharSequence_64(2, str), \
                                   MakeCharSequence_64(3, str), \
                                   MakeCharSequence_64(4, str)

#define STATIC_STRING_16(str) Arrow::Splite<sizeof(str) - 1, Arrow::ValueTypeList<char, MakeCharSequence_16(str)>>::Head
#define STATIC_STRING_64(str) Arrow::Splite<sizeof(str) - 1, Arrow::ValueTypeList<char, MakeCharSequence_64(str)>>::Head
#define STATIC_STRING_128(str) Arrow::Splite<sizeof(str) - 1, Arrow::ValueTypeList<char, MakeCharSequence_128(str)>>::Head
#define STATIC_STRING_256(str) Arrow::Splite<sizeof(str) - 1, Arrow::ValueTypeList<char, MakeCharSequence_256(str)>>::Head
#define STATIC_STRING_512(str) Arrow::Splite<sizeof(str) - 1, Arrow::ValueTypeList<char, MakeCharSequence_512(str)>>::Head
#define STATIC_STRING_1024(str) Arrow::Splite<sizeof(str) - 1, Arrow::ValueTypeList<char, MakeCharSequence_1024(str)>>::Head
#define STATIC_STRING(str) STATIC_STRING_1024(str)

#ifdef WIN32
#define GET_FILE_NAME(fullFileName) Arrow::StaticStr::SubStr<Arrow::StaticStr::FindLast(fullFileName, '\\') + 1>(fullFileName)
#else
#define GET_FILE_NAME(fullFileName) Arrow::StaticStr::SubStr<Arrow::StaticStr::FindLast(fullFileName, '/') + 1>(fullFileName)
#endif

#define __ARROW_FILE_NAME_VALUE__ (GET_FILE_NAME(__FILE__).data)
#define __ARROW_FILE_NAME_TYPE__ Arrow::Splite<Arrow::StaticStr::FindLast(__FILE__, '/') + 1, STATIC_STRING(__FILE__)>::Tail

#define __ARROW_FUN_NAME_VALUE__ Arrow::StaticStr::Str(__func__)
#define __ARROW_FUN_NAME_TYPE__ STATIC_STRING(__func__)