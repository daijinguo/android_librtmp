#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <initializer_list>
#include <unordered_set>

namespace dai {

template<typename T>
class SafeQueue {
private:
    mutable std::mutex              mutex;
    mutable std::condition_variable cv;

    using QueueType = std::queue<T>;
    QueueType dataQueue;

public:
    using value_type     = typename QueueType::value_type;
    using container_type = typename QueueType::container_type;

    SafeQueue()                             = default;
    SafeQueue(const SafeQueue &)            = delete;
    SafeQueue &operator=(const SafeQueue &) = delete;

    template<typename IT>
    SafeQueue(IT first, IT last)
    {
        for (auto iterator = first; iterator != last; ++iterator)
        {
            dataQueue.push(*iterator);
        }
    }

    explicit SafeQueue(const container_type &ct) : dataQueue(ct)
    {
    }

    SafeQueue(std::initializer_list<value_type> list) : SafeQueue(list.begin(), list.end())
    {
    }

    void push(const value_type &value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        dataQueue.push(std::move(value));
        cv.notify_one();
    }

    value_type wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait( lock, [this]{ return !this->dataQueue.empty(); } );
        auto value = std::move(dataQueue.front());
        dataQueue.pop();
        return value;
    }

    bool try_pop(value_type &value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (dataQueue.empty())
            return false;
        value = std::move(dataQueue.front());
        dataQueue.pop();
        return true;
    }

    auto empty() const -> decltype(dataQueue.empty())
    {
        std::lock_guard<std::mutex> lock(mutex);
        return dataQueue.empty();
    }

    auto size() const -> decltype(dataQueue.size())
    {
        std::lock_guard<std::mutex> lock(mutex);
        return dataQueue.size();
    }

    void drop(std::unordered_set<value_type> &result_set)
    {
        std::lock_guard<std::mutex> lock(mutex);
        while (!dataQueue.empty())
        {
            dataQueue.pop();
            result_set.erase(std::move(dataQueue.front()));
        }
    }
};

}
