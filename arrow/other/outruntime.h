#include <thread>
#include <list>
#include <chrono>
namespace Arrow
{

namespace Other
{

class CRunTime
{
public:
    CRunTime(const char* szPrefix)
    {
        m_ListClock.push_back(std::clock());
        m_strPrefix = szPrefix;
    }

    void Add()
    {
        m_ListClock.push_back(std::clock());
    }

    ~CRunTime()
    {
        auto it1 = m_ListClock.begin();
        auto it2 = ++(m_ListClock.begin());

        int nIndex = 1;
        std::cout << m_strPrefix << " ";
        for (; it2 != m_ListClock.end(); ++it1, ++it2, ++nIndex)
        {
            std::cout << "Time-" << nIndex << ":" << (*it2 - *it1) * 1000 / CLOCKS_PER_SEC << " ";
        }
        std::cout << " Time-" << nIndex << ":" << (std::clock() - *it1) * 1000 / CLOCKS_PER_SEC << " ";
        std::cout << "All Time:" << (std::clock() - *m_ListClock.begin()) * 1000 / CLOCKS_PER_SEC << std::endl;
    }
private:
    std::list<std::clock_t> m_ListClock;
    std::string m_strPrefix;
};

}
}