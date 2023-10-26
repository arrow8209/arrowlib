#pragma once
#include <list>
#include <mutex>
#include <any>

template <typename T>
class TaskQueueT
{
    using List_Task = std::list<T>; // 缓冲区，存储所有的任务 [zhuyb 2022-07-05 09:00:43]

public:
    TaskQueueT() {}
    virtual ~TaskQueueT()
    {
    }

    std::any tmp;
};