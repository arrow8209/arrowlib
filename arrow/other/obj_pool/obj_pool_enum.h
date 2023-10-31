#pragma once

namespace Arrow
{

namespace Other
{

// 对象池空闲存活时间 单位:秒 [zhuyb 2022-10-24 08:57:04]
typedef enum _em_ObjLiveTime
{
    _em_objLiveMax = 0,
    _em_ObjLiv30 = 30,
    _em_ObjLive60 = 60,
    _em_ObjLive90 = 90,
    _em_ObjLive120 = 120,
    _em_ObjLive240 = 240,
    _em_ObjLive480 = 480,
    _em_ObjLive960 = 960,
    _em_ObjLive1920 = 1920
} Em_ObjLiveTime;

}

}