/*
 * @FilePath: /ai_server/ShareCode/arrow/pattern/observer/observer.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:27:10
 * @Description: 观察者接收类，这里还有一种实现方式就是对 Response 函数进行模板化（还未进行尝试这种方法）
 */
#pragma once

namespace Arrow
{

namespace Pattern
{


template <typename _Ret, typename ..._Args>
class Observer
{
public:
    Observer() {}

    virtual ~Observer() {}

public:
    virtual _Ret Response(_Args... _args) = 0;
};

//Demo
// class UserClass : public IObserver<parem1, parem2, pamre3 ... parmen>
// {
//     virtual parem1 Response(parem2, pamre3 ... parmen)
//     {
//         .....
//     }
// }

}
}