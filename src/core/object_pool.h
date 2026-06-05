#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>

//////////////////////////////////////////////////////
template<typename T>
class ObjectPool {
private:
    std::vector<T*> available;
    std::vector<T*> inUse;
    int capacity;
/////////////////////////////////////////////////////
public:
    explicit ObjectPool(int cap) : capacity(cap) {
        for (int i = 0; i < cap; i++)
            available.push_back(new T());
    }

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    ~ObjectPool() {
        for (T* obj : available) delete obj;
        for (T* obj : inUse) delete obj;
    }

    T* acquire() {
        if (available.empty())
            throw std::runtime_error("ObjectPool: no objects available");            ////////////throw
        T* obj = available.back();
        available.pop_back();
        inUse.push_back(obj);
        return obj;
    }

    void release(T* obj) {
        for (auto it = inUse.begin(); it != inUse.end(); ++it) {
            if (*it == obj) {
                inUse.erase(it);
                available.push_back(obj);
                return;
            }
        }
        throw std::runtime_error("ObjectPool: object not from this pool");
    }

    int getAvailable() const { return (int)available.size(); }
    int getInUse() const { return (int)inUse.size(); }
    int getCapacity() const { return capacity; }

    friend std::ostream& operator<<(std::ostream& os, const ObjectPool<T>& p) {
        os << "ObjectPool<T> capacity=" << p.capacity
           << " available=" << p.available.size()
           << " inUse=" << p.inUse.size();
        return os;
    }
};
