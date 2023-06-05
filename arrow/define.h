#pragma once

#ifdef WIN32
    #define ARROW_API_EXPORT _declspec(dllexport)
    #define ARROW_API_IMPORT _declspec(dllimport)
#else
    #define ARROW_API_EXPORT __attribute__((visibility("default")))
    #define ARROW_API_IMPORT
    #define ARROW_ARI ARROW_API_EXPORT
#endif

