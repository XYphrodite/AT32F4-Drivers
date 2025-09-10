#pragma once
#include <cstddef>
#include <stdexcept>

// A simple static array class
// Usage: BigBeautifulStaticArray<int, 10> arr;
template <typename T, std::size_t N>
class BigBeautifulStaticArray
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using iterator = T *;
    using const_iterator = const T *;

    BigBeautifulStaticArray()
    {
        count_ = 0;
    }

    // Element access
    T &operator[](size_type idx) { return data_[idx]; }
    const T &operator[](size_type idx) const { return data_[idx]; }

    T *at(size_type idx)
    {
        if (idx >= count_)
            return nullptr;
        return &data_[idx];
    }
    const T *at(size_type idx) const
    {
        if (idx >= count_)
            return nullptr;
        return &data_[idx];
    }

    // Capacity
    constexpr size_type capacity() const { return N; }
    size_type size() const { return count_; }
    bool empty() const { return count_ == 0; }

    // Modifiers
    void clear() { count_ = 0; }

    void push_back(const T &value)
    {
        if (count_ >= N)
            return;
        data_[count_++] = value;
    }

    void push_back(const T *values, size_type count)
    {
        for (size_type i = 0; i < count && this->count_ < N; ++i)
        {
            data_[this->count_++] = values[i];
        }
    }

    void erase(size_type idx)
    {
        if (idx >= count_)
            return;
        for (size_type i = idx; i < count_ - 1; ++i)
            data_[i] = data_[i + 1];
        --count_;
    }

    void erase(iterator it)
    {
        erase(it - begin());
    }

    // Iterators
    iterator begin() { return data_; }
    iterator end() { return data_ + count_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + count_; }
    const_iterator cbegin() const { return data_; }
    const_iterator cend() const { return data_ + count_; }

    // Fill
    void fill(const T &value)
    {
        for (size_type i = 0; i < N; ++i)
            data_[i] = value;
        count_ = N;
    }

    // Data access
    T *data() { return data_; }
    const T *data() const { return data_; }

private:
    T data_[N ? N : 1]; // Avoid zero-length array
    size_type count_ = 0;
};
