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

typedef enum _em_call_type
{
    _stdcall_ = 0,
    _cdecl_ = 1,
    _fastcall_ = 2
}EmCallType;

typedef enum _em_ErrorCode
{
    _em_Success = 0,
    _em_LoadLibFail,
    _em_UnloadLib,
    _em_FindFunFail,
} Em_ErrorCode;


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
        m_hInst = nullptr;
        m_ErrorNo = Em_ErrorCode::_em_Success;
        m_strLastErrorMsg = "";
    }

    DynamicLib(const char* lpLibFileName) // 构造函数,调用HLoadLibrary加载DLL
    {
        m_hInst = nullptr;
        m_hInst = LoadLib(lpLibFileName);
    }

    virtual ~DynamicLib()
    {
        if (m_hInst != nullptr)
        {
            CloseLib(m_hInst);
            m_hInst = nullptr;
        }
    }

    DynamicLibHandle LoadLib(const char* lpLibFileName) // 加载DLL
    {
        if (m_hInst != nullptr)
        {
            CloseLib(m_hInst);
            m_hInst = nullptr;
        }
        m_hInst = OpenLib(lpLibFileName);
        if (m_hInst == nullptr)
        {
            m_ErrorNo = Em_ErrorCode::_em_LoadLibFail;

            std::ostringstream oss;
            oss << lpLibFileName << " Load Fail";
            m_strLastErrorMsg = oss.str();

            std::cout << m_strLastErrorMsg << std::endl;
            return nullptr;
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
        if (m_hInst == nullptr)
            return false;
        char* tmpProcName = const_cast<char*>(lpPorcName);
        DynamicLibFunAddress pFunAddress = GetFunAddress(m_hInst, tmpProcName);
        return pFunAddress != nullptr;
    }

    bool CheckOpenLib()
    {
        return m_hInst != nullptr;
    }

    const char* LastErrorMsg()
    {
        return m_strLastErrorMsg.c_str();
    }

    Em_ErrorCode LastErrorCode()
    {
        return m_ErrorNo;
    }
public:

    /**
     * @description: 主要给Windows使用，CallCon 确定调用约定，Linux下使用，会忽略这个函数, 注意：会有异常抛出
     * @param {char*} lpProcName
     * @param {Args...} args
     * @return {*}
     */
    template <EmCallType CallCon, typename RetType, typename... Args>
    RetType call(const char* lpProcName, Args... args)
    {
        return call_win<RetType>(lpProcName, type<CallCon>(), args...);
    }

    // 主要给Linux使用，没有调用约定约束，Windows也可以使用，默认使用编译器默认调用约定 [zhuyb 2022-07-18 14:36:45]
    /**
     * @description: 主要给Linux使用，没有调用约定约束，Windows也可以使用，默认使用编译器默认调用约定 注意：会有异常抛出
     * @param {char*} lpProcName
     * @param {Args...} args
     * @return {*}
     */
    template <typename RetType, typename... Args>
    RetType call(const char* lpProcName, Args... args)
    {
        typedef RetType(* LPProcName)(Args...);
        return addr<LPProcName>(lpProcName)(args...);
        // 在windows下，可以使用下面的代码，使用特定的默认调用 [zhuyb 2023-01-11 08:46:12]
        //return call_win<RetType>(lpProcName, type<EmCallType::_stdcall_>(), args...);
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
    RetType call_win(const char* lpProcName, type<_stdcall_>, Args... args)
    {
        typedef RetType(__stdcall * LPProcName)(Args...);
        return addr<LPProcName>(lpProcName)(args...);
    }

    //////////////////////////////////////////////////////////////////////////
    // 对__cdecl调用约定的实现
    template <class RetType, typename... Args>
    RetType call_win(const char* lpProcName, type<_cdecl_>, Args... args)
    {
        typedef RetType(__cdecl * LPProcName)(Args...);
        return addr<LPProcName>(lpProcName)(args...);
    }

    /////////////////////////////////////////////////////////////////////////
    // 对__fastcall调用约定的实现
    template <class RetType, typename... Args>
    RetType call_win(const char* lpProcName, type<_fastcall_>, Args... args)
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
        if (hin == nullptr)
        {
            return 0;
        }
        
        int nRet = 0;
#ifdef WIN32
        nRet = FreeLibrary(hin);
#else
        nRet = dlclose(hin);
#endif // WIN32
        m_hInst = nullptr;
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