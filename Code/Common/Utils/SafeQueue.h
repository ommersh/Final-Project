#pragma once
#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H
#include <queue>
#include <mutex>
template<typename T>
class SafeQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mtx;

public:
    SafeQueue() {}

    // Prevent copy construction and assignment
    SafeQueue(const SafeQueue& other) = delete;
    SafeQueue& operator=(const SafeQueue& other) = delete;

    void enqueue(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
    }

    bool dequeue(T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty()) {
            return false;
        }
        value = std::move(queue.front());
        queue.pop();
        return true;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
};

#endif