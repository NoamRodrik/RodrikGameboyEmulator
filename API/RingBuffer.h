/**
 * @file		RingBuffer.h
 * @author		Phillip Johnston, modified by Noam Rodrik
 * @description https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc
 */
#ifndef __API_RING_BUFFER_H__
#define __API_RING_BUFFER_H__

#include <Tools/Tools.h>
#include <cstdio>
#include <memory>
#include <atomic>

namespace API
{
template <typename T, size_t N>
class [[nodiscard]] RingBuffer
{
    // Static assertion that N is a multiple of 2 for efficient modulo.
    static_assert(N % 2 == 0, "N must be a multiple of 2");

public:
    RingBuffer() = default;

    template <typename U>
    void Push(U&& item)
    {
        this->_buffer[this->_head] = std::forward<T>(item);

        if (this->IsFull())
        {
            this->_tail = (this->_tail + 1) & (N - 1);
        }

        this->_head = (this->_head + 1) & (N - 1);

        this->_full = this->_head == this->_tail;
    }

    [[nodiscard]] T Pop()
    {
        T return_value{};

        if (!this->IsEmpty())
        {
            // Read data and advance the tail (we now have a free space)
            return_value = std::move(this->_buffer[this->_tail]);
            this->_full = false;
            this->_tail = (this->_tail + 1) & (N - 1);
        }

        return return_value;
    }

    [[nodiscard]] const size_t GetCapacity() const
    {
        return this->IsFull() ? N : (((this->_head < this->_tail) * N) + (this->_head - this->_tail));
    }

private:
    [[nodiscard]] const bool IsEmpty() const
    {
        // If head and tail are equal, we are empty
        return (!this->IsFull() && (this->_head == this->_tail));
    }

    [[nodiscard]] constexpr bool IsFull() const
    {
        // If tail is ahead the head by 1, we are full
        return this->_full;
    }

private:
    std::array<T, N>         _buffer{};
    std::atomic<std::size_t> _head{0};
    std::atomic<std::size_t> _tail{0};
    std::atomic<bool>        _full{false};
};
} // API

#endif // __API_RING_BUFFER_H__