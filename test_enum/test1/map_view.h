#pragma once
#include <map>

template<typename Key, typename Value>
struct map_view : public std::map<Key, Value>
{
    constexpr map_view() {}
};

void TestMapView()
{
    map_view<int, int> mv;
    std::cout << mv.size() << std::endl;
}