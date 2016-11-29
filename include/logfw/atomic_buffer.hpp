/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_atomic_buffer_291116164839_MADLIFE
#define MADLIFE_atomic_buffer_291116164839_MADLIFE

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <array>
#include "compiler.hpp"

namespace logfw {

/* buffer byte type */
using u8 = uint8_t;

/** wrapper around buffer */
class atomic_buffer final
{
private:
    /* pointer to buffer data */
    u8* ptr_{nullptr};
    /* size of buffer data */
    std::size_t size_{0};

public:
    /** default constructor */
    atomic_buffer() = default;

    /** destructor */
    ~atomic_buffer() = default;

    /** move constructor */
    atomic_buffer(atomic_buffer&& buffer)
        : ptr_(buffer.ptr_)
        , size_(buffer.size_)
    {}

    /** move operator */
    atomic_buffer& operator=(atomic_buffer&& buffer)
    {
        ptr_ = buffer.ptr_;
        size_ = buffer.size_;
        return *this;
    }

    /** construct buffer by wrapping raw pointer */
    atomic_buffer(void* ptr, std::size_t size)
        : ptr_(reinterpret_cast< u8* >(ptr))
        , size_(size)
    {}

    /** construct buffer from an array */
    template< std::size_t N >
    atomic_buffer(std::array< char, N >& buffer)
        : ptr_(buffer.data())
        , size_(N)
    {}

    /** construct buffer from a c-style array */
    template< std::size_t N >
    atomic_buffer(char (&buffer)[N])
        : ptr_(reinterpret_cast< u8* >(buffer))
        , size_(N)
    {}

    /** wrap other buffer */
    __force_inline void wrap(const atomic_buffer& buffer)
    {
        ptr_ = buffer.ptr_;
        size_ = buffer.size_;
    }

    /** wrap raw pointer */
    __force_inline void wrap(void* ptr, std::size_t size)
    {
        ptr_ = reinterpret_cast< u8* >(ptr);
        size_ = size;
    }

    /** wrap an array */
    template< std::size_t N >
    __force_inline void wrap(std::array< char, N >& buffer)
    {
        ptr_ = reinterpret_cast< u8* >(buffer.data());
        size_ = N;
    }

    /** wrap a c-style array */
    template< std::size_t N >
    __force_inline void wrap(char (&buffer)[N])
    {
        ptr_ = reinterpret_cast< u8* >(buffer);
        size_ = N;
    }

    /** return buffer capacity */
    __force_inline std::size_t capacity() const { return size_; }

    /** change buffer capacity */
    __force_inline void capacity(std::size_t size) { size_ = size; }

    /** return const pointer to buffer data from desired offset */
    __force_inline const u8* data(std::size_t offset = 0) const { return ptr_ + offset; }

    /** return pointer to buffer data from desired offset */
    __force_inline u8* data(std::size_t offset = 0) { return ptr_ + offset; }

    /** return buffer as desired type from desired position */
    template< class TypeT >
    __force_inline TypeT& as(std::size_t offset = 0)
    {
        bounds_check(offset, sizeof(TypeT));
        return *reinterpret_cast< TypeT* >(data(offset));
    }

    /** return buffer as desired type from desired position */
    template< class TypeT >
    __force_inline const TypeT& as(std::size_t offset = 0) const
    {
        bounds_check(offset, sizeof(TypeT));
        return *reinterpret_cast< const TypeT* >(data(offset));
    }

private:
    /* bounds check */
    __force_inline void bounds_check(std::size_t pos, std::size_t length) const
    {
        if (__unlikely(!((pos + length) <= size_))) {
            throw std::range_error("out of bounds check failed");
        }
        /* TODO: add ability to throw or assert */
    }
};

} /* namespace logfw */

#endif /* MADLIFE_atomic_buffer_291116164839_MADLIFE */
