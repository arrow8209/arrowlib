#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#define __stdcall
#define __cdecl
#define __fastcall
#endif

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace Arrow
{

namespace Other
{
// Linux Windows 类型适配  [zhuyb 2022-07-18 10:58:15]
#ifdef WIN32
typedef HMODULE DynamicLibHandle;
typedef FARPROC DynamicLibFunAddress;
#else
typedef void* DynamicLibHandle;
typedef void* DynamicLibFunAddress;
#endif

enum _em_call_type
{
    _calltype_stdcall = 0,
    _calltype_cdecl = 1,
    _fastcall_ = 2
};

typedef enum _em_ErrorCode
{
    _em_Success = 0,
    _em_LoadLibFail,
    _em_UnloadLib,
    _em_FindFunFail,
} Em_ErrorCode;

static inline std::string get_app_path()
{
#ifdef WIN32
    char chPath[MAX_PATH];
    std::string strPath;
    int nCount;

    ::GetModuleFileName(NULL, chPath, MAX_PATH); //得到执行文件名的全路径
    strPath = chPath;
    nCount = strPath.find_last_of('\\');
    strPath = strPath.substr(0, nCount + 1);
    return strPath;

#else
    std::string sPath;
    char buf[4097] = {0};
    long size;
    char* ptr;
    size = pathconf(".", _PC_PATH_MAX);
    if ((ptr = (char*)malloc((size_t)size)) != NULL)
    {
        memset(ptr, 0, size);
        sprintf(ptr, "/proc/%d/exe", getpid());
    }
    else
        return sPath;
    readlink(ptr, buf, size);
    free(ptr);
    ptr = NULL;
    sPath = buf;
    int nPos = sPath.find_last_of("/");
    return sPath.substr(0, nPos + 1);
#endif
}

class DynamicLib
{

public:
    //////////////////////////////////////////////////////////////////////////
    // 辅助类,用一个整数映射到一个型别
    template <int index>
    struct type{};

public:
    DynamicLib()
    {
        m_hInst = NULL;
        m_ErrorNo = Em_ErrorCode::_em_Success;
        m_strLastErrorMsg = "";
    }

    DynamicLib(const char* lpLibFileName) // 构造函数,调用HLoadLibrary加载DLL
    {
        m_hInst = NULL;
        m_hInst = LoadLib(lpLibFileName);
    }

    virtual ~DynamicLib()
    {
        CloseLib(m_hInst);
    }

    DynamicLibHandle LoadLib(const char* lpLibFileName) // 加载DLL
    {
        if (m_hInst != NULL)
        {
            CloseLib(m_hInst);
            m_hInst = NULL;
        }
        m_hInst = OpenLib(lpLibFileName);
        if (m_hInst == NULL)
        {
            m_ErrorNo = Em_ErrorCode::_em_LoadLibFail;

            std::ostringstream oss;
            oss << lpLibFileName << " Load Fail";
            m_strLastErrorMsg = oss.str();

            std::cout << m_strLastErrorMsg << std::endl;
            throw std::runtime_error(std::string(lpLibFileName));
        }
        m_strLibName = lpLibFileName;
        return m_hInst;
    }

    int CloseLib()
    {
        return CloseLib(m_hInst);
    }

    bool CheckFun(const char* lpPorcName)
    {
        if (m_hInst == NULL)
            return false;
        char* tmpProcName = const_cast<char*>(lpPorcName);
        DynamicLibFunAddress pFunAddress = GetFunAddress(m_hInst, tmpProcName);
        return pFunAddress != NULL;
    }

    bool CheckOpenLib()
    {
        return m_hInst != NULL;
    }

public:
    // 主要给Linux使用，没有调用约定约束，Windows也可以使用，默认使用__stdcall调用约定 [zhuyb 2022-07-18 14:36:45]
    template <typename RetType, typename... Args>
    RetType call(const char* lpProcName, Args... args)
    {
        typedef RetType (__stdcall *LPProcName)(Args...);
        return addr<LPProcName>(lpProcName)(args...);
    }

    // 主要给Windows使用，CallCon 确定调用约定，Linux下使用，会忽略这个参数 [zhuyb 2022-07-18 14:36:45]
    template <typename RetType, int CallCon, typename... Args>
    RetType call(const char* lpProcName, Args... args)
    {
        return call<RetType>(lpProcName, type<CallCon>(), args...);
    }

private:
    template <class FunType>
    FunType addr(const char* lpProcName) // 得到指定函数类型的地址
    {
        FunType funaddr = (FunType) nullptr;

        if (m_hInst == nullptr)
        {
            m_ErrorNo = Em_ErrorCode::_em_UnloadLib;
            m_strLastErrorMsg = "Don't load lib";
            std::cout << m_strLastErrorMsg << std::endl;
            throw std::runtime_error(m_strLastErrorMsg);
        }
        else
        {
            // char* tmpProcName = const_cast<char*>(lpProcName);
            funaddr = (FunType)GetFunAddress(m_hInst, lpProcName);
            if (funaddr == (FunType) nullptr)
            {
                m_ErrorNo = Em_ErrorCode::_em_FindFunFail;
                std::ostringstream oss;
                oss << "Find " << lpProcName << " fun fail";
                m_strLastErrorMsg = oss.str();

                std::cout << m_strLastErrorMsg << std::endl;
                throw std::runtime_error(m_strLastErrorMsg);
            }
        }

        return funaddr;
    }

    //////////////////////////////////////////////////////////////////////////
    // 对__stdcall调用约定的实现
    template <class RetType, typename... Args>
    RetType call(const char* lpProcName, type<_calltype_stdcall>, Args... args)
    {
        typedef RetType(__stdcall * LPProcName)(Args...);
        return addr<LPProcName>(lpProcName)(args...);
    }

    //////////////////////////////////////////////////////////////////////////
    // 对__cdecl调用约定的实现
    template <class RetType, typename... Args>
    RetType call(const char* lpProcName, type<_calltype_cdecl>, Args... args)
    {
        typedef RetType(__cdecl * LPProcName)(Args...);
        return addr<LPProcName>(lpProcName)(args...);
    }

    /////////////////////////////////////////////////////////////////////////
    // 对__fastcall调用约定的实现
    template <class RetType, typename... Args>
    RetType call(const char* lpProcName, type<_fastcall_>, Args... args)
    {
        typedef RetType(__fastcall * LPProcName)(Args...);
        return addr<LPProcName>(lpProcName)(args...);
    }

private:
    DynamicLibHandle OpenLib(const char* filename)
    {
#ifdef WIN32
        return LoadLibrary(filename);
#else
        return dlopen(filename, RTLD_NOW);
#endif // WIN32
    }

    int CloseLib(DynamicLibHandle& hin)
    {
        int nRet = 0;
#ifdef WIN32
        nRet = FreeLibrary(hin);
#else
        nRet = dlclose(hin);

#endif // WIN32
        m_hInst = NULL;
        return nRet;
    }

    DynamicLibFunAddress GetFunAddress(DynamicLibHandle& hin, const char* funName)
    {
#ifdef WIN32
        return GetProcAddress(hin, funName);
#else
        return dlsym(hin, funName);
#endif
    }

private:
    DynamicLibHandle m_hInst;
    Em_ErrorCode m_ErrorNo;
    std::string m_strLibName;
    std::string m_strLastErrorMsg;
};

}
}