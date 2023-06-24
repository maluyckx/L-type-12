#ifndef LTYPE_UTILS_THREADSAFEQUEUE_H
#define LTYPE_UTILS_THREADSAFEQUEUE_H

#include <queue>
#include <pthread.h>

namespace Utils {
    /**
     * Thread safe implementation of a queue
     * @tparam T is the type of the element the queue is storing
     */
    template<typename T>
    class ThreadSafeQueue {
    private:
        std::queue<T>  * _queue = new std::queue<T>();
        pthread_mutex_t* _mutex = new pthread_mutex_t();
    public:
        constexpr ThreadSafeQueue();
        ThreadSafeQueue(const ThreadSafeQueue& other);
        ThreadSafeQueue(ThreadSafeQueue&& other) noexcept;
        ThreadSafeQueue<T>& operator=(const ThreadSafeQueue& rhs);
        ThreadSafeQueue<T>& operator=(ThreadSafeQueue&& rhs) noexcept;
        ~ThreadSafeQueue() noexcept;
        void push(T item);
        T pop();
        const T& peek() const;
        [[nodiscard]] size_t size() const;
        bool empty();
        void clear();
    };

    template<typename T>
    constexpr ThreadSafeQueue<T>::ThreadSafeQueue() {
        pthread_mutex_init(_mutex, nullptr);
    }

    template<typename T>
    ThreadSafeQueue<T>::ThreadSafeQueue(const ThreadSafeQueue& other):
            _queue(other._queue) {
        pthread_mutex_init(_mutex, nullptr);
    }

    template<typename T>
    ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue&& other) noexcept:
            _queue(std::move(other._queue)) {
        pthread_mutex_init(_mutex, nullptr);
    }

    template<typename T>
    ThreadSafeQueue<T>&
    ThreadSafeQueue<T>::operator=(const ThreadSafeQueue& rhs) {
        if (this != &rhs) {
            _queue = rhs._queue;
            pthread_mutex_init(_mutex, nullptr);
        }
    }

    template<typename T>
    ThreadSafeQueue<T>&
    ThreadSafeQueue<T>::operator=(ThreadSafeQueue&& rhs) noexcept {
        _queue = rhs._queue;
        _mutex = rhs._mutex;
        rhs._queue = nullptr;
        rhs._mutex = nullptr;
        return *this;
    }

    template<typename T>
    ThreadSafeQueue<T>::~ThreadSafeQueue() noexcept {
        pthread_mutex_destroy(_mutex);
        delete _queue;
    }

    template<typename T>
    inline void ThreadSafeQueue<T>::push(T item) {
        pthread_mutex_lock(_mutex);
        _queue->push(item);
        pthread_mutex_unlock(_mutex);
    }

    template<typename T>
    inline T ThreadSafeQueue<T>::pop() {
        pthread_mutex_lock(_mutex);
        T item = _queue->front();
        _queue->pop();
        pthread_mutex_unlock(_mutex);
        return item;
    }

    template<typename T>
    inline const T& ThreadSafeQueue<T>::peek() const {
        pthread_mutex_lock(_mutex);
        const T& item = _queue->front();
        pthread_mutex_unlock(_mutex);
        return item;
    }

    template<typename T>
    inline size_t ThreadSafeQueue<T>::size() const {
        pthread_mutex_lock(_mutex);
        auto size = _queue->size();
        pthread_mutex_unlock(_mutex);
        return size;
    }

    template<typename T>
    inline bool ThreadSafeQueue<T>::empty() {
        pthread_mutex_lock(_mutex);
        auto res = _queue->empty();
        pthread_mutex_unlock(_mutex);
        return res;
    }

    template<typename T>
    inline void ThreadSafeQueue<T>::clear() {
        pthread_mutex_lock(_mutex);
        while (!_queue->empty()) { _queue->pop(); }
        pthread_mutex_unlock(_mutex);
    }
}

#endif
