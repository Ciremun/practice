#include <list>
#include <unordered_map>
#include <cstdio>

template <class T>
struct LRU
{
    std::list<T> list;
    std::unordered_map<T, decltype(list.begin())> map;
    size_t capacity;

    LRU(size_t capacity) : capacity(capacity) {}

    void refer(T key)
    {
        if (map.find(key) == map.end())
        {
            if (list.size() == capacity)
            {
                T last = list.back();
                list.pop_back();
                map.erase(last);
            }
        }
        else
        {
            list.erase(map[key]);
        }
        list.push_front(key);
        map[key] = list.begin();
    }
};

int main()
{
    LRU<size_t> lru(4);

    lru.refer(1);
    lru.refer(2);
    lru.refer(3);
    lru.refer(1);
    lru.refer(4);
    lru.refer(5);

    for (auto &item : lru.list)
    {
        printf("%ld ", item);
    }
    putc('\n', stdout);

    return 0;
}