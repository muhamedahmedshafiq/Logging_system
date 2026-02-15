#pragma once
#include <vector>
#include <optional>
#include <mutex>

template <typename T>
class RingBuffer {
private:
    std::vector<std::optional<T>> buffer;
    size_t head = 0; 
    size_t tail = 0;
    size_t size;
    mutable std::mutex mtx;

public:
    explicit RingBuffer(size_t capacity) : size(capacity + 1) {
        buffer.resize(size);
    }

    // Delete Copy Semantics
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;

    // Move Semantics
    RingBuffer(RingBuffer&& other) noexcept = default;
    RingBuffer& operator=(RingBuffer&& other) noexcept = default;

    bool tryPush(T&& item) {
        std::lock_guard<std::mutex> lock(mtx);
        size_t next = (head + 1) % size;
        if (next == tail) 
        {
            return false; // Buffer full
        }

        buffer[head] = std::move(item);
        head = next;
        return true;
    }

    std::optional<T> tryPop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (head == tail){
            return std::nullopt; // Buffer empty
        }
         
        std::optional<T> item = std::move(buffer[tail]);
        buffer[tail] = std::nullopt;
        tail = (tail + 1) % size;
        return item;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return head == tail;
    }

    size_t capacity() const {
        return size - 1;
    }
};