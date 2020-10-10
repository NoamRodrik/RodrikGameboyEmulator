/**
 * @file		RingBuffer.h
 * @author		Noam Rodrik
 * @description https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc
 */
#ifndef __API_RING_BUFFER_H__
#define __API_RING_BUFFER_H__

#include <cstdio>
#include <memory>
#include <mutex>

namespace API
{
template <typename T>
class RingBuffer 
{
public:
    explicit RingBuffer(const std::size_t size) :
        _buffer{std::unique_ptr<T[]>{new T[size]}},
        _max_size{size} {}

    template <typename U>
    void Push(U&& item)
    {
        std::lock_guard<std::mutex> lock{this->_mutex};

        this->_buffer[this->_head] = std::forward<T>(item);

        if (this->_full)
        {
            this->_tail = (this->_tail + 1) % this->_max_size;
        }

        this->_head = (this->_head + 1) % this->_max_size;

        this->_full = this->_head == this->_tail;
    }

    [[nodiscard]] T Pop()
    {
        std::lock_guard<std::mutex> lock{this->_mutex};
        T return_value{};

        if (!this->IsEmpty())
        {
            // Read data and advance the tail (we now have a free space)
            return_value = std::move(this->_buffer[this->_tail]);
            this->_full = false;
            this->_tail = (this->_tail + 1) % this->_max_size;
        }

        return return_value;
    }

    void Reset()
    {
        std::lock_guard<std::mutex> lock{this->_mutex};
        this->_head = this->_tail;
        this->_full = false;
    }

    [[nodiscard]] const bool IsEmpty() const
    {
        // If head and tail are equal, we are empty
        return (!this->_full && (this->_head == this->_tail));
    }

    [[nodiscard]] const bool IsFull() const
    {
        // If tail is ahead the head by 1, we are full
        return this->_full;
    }

    const size_t GetCapacity() const
    {
        return this->_max_size;
    }

    const size_t GetSize() const
    {
        size_t size = this->_max_size;

        if (!this->_full)
        {
            if (this->_head >= this->_tail)
            {
                size = this->_head - this->_tail;
            }
            else
            {
                size = this->_max_size + this->_head - this->_tail;
            }
        }

        return size;
    }

private:
    std::mutex _mutex{};
    std::unique_ptr<T[]> _buffer{nullptr};
    size_t _head{0};
    size_t _tail{0};
    const size_t _max_size;
    bool _full{false};
};
} // API

#endif // __API_RING_BUFFER_H__