#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H
#include <deque>
#include <mutex>

template<typename T>
/// <summary>
/// Implementation of a generic queue with limited access using a mutex.
/// Used for queues shared between threads
/// </summary>
class SafeQueue {
private:
    std::deque<T> queue;
    mutable std::mutex mtx;

public:
    SafeQueue() {}

    // Prevent copy construction and assignment
    SafeQueue(const SafeQueue& other) = delete;
    SafeQueue& operator=(const SafeQueue& other) = delete;

    /// <summary>
    /// Add to queue
    /// </summary>
    /// <param name="value"></param>
    void enqueue(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push_back(std::move(value));
    }

    /// <summary>
    /// Return something to the top of the queue
    /// </summary>
    /// <param name="value"></param>
    void returnToTopOFQueue(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push_front(std::move(value));
    }

    /// <summary>
    /// Remove the top object from the queue
    /// </summary>
    /// <param name="value"></param>
    /// <returns></returns>
    bool dequeue(T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty()) {
            return false;
        }
        value = std::move(queue.front());
        queue.pop_front();
        return true;
    }

    /// <summary>
    /// check if the queue is empty
    /// </summary>
    /// <returns></returns>
    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }

    /// <summary>
    /// Return the queue size
    /// </summary>
    /// <returns></returns>
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
};

#endif