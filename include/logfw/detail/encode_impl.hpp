/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_encode_impl_021216225339_MADLIFE
#define MADLIFE_encode_impl_021216225339_MADLIFE

#include <cstring>
#include <type_traits>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <limits>
#include "../common.hpp"

namespace logfw {
namespace detail {

/**
 * Maximum length of loggable string.
 * Strings will be trimed in case of greater 256 chars.
 */
static constexpr const std::size_t string_max_length = std::numeric_limits< uint8_t >::max();

/** Argument i/o handlers */
template< class T, class Enable = void >
struct arg_io;

template< class T >
struct arg_io< T, typename std::enable_if< std::is_arithmetic< T >::value >::type >
{
    /** @return Maximum numbers of bytes to store the type in the buffer */
    static constexpr std::size_t max_bytes_required() noexcept
    {
        return sizeof(T);
    }

    /** @return Numbers of actual bytes required for store the arg */
    static constexpr std::size_t bytes_required(const T& value) noexcept
    {
        return sizeof(T);
    }

    /**
     * Copy type to buffer.
     * @return Used bytes.
     */
    LOGFW_FORCE_INLINE static std::size_t encode(T value, char* buffer) noexcept
    {
        *reinterpret_cast< T* >(buffer) = value;
        return sizeof(T);
    }

    /**
     * Copy type from buffer.
     * @return Used bytes.
     */
    LOGFW_FORCE_INLINE static std::size_t decode(T& value, const char* buffer, size_t size)
    {
        if (LOGFW_UNLIKELY(size < sizeof(T))) {
            throw std::runtime_error("Buffer too small");
        }
        value = *reinterpret_cast< const T* >(buffer);
        return sizeof(T);
    }
};

template<>
struct arg_io< string_view >
{
    /** @return Maximum numbers of bytes to store the type in the buffer. */
    static constexpr std::size_t max_bytes_required() noexcept
    {
        return string_max_length + 1;
    }

    /** @return Numbers of actual bytes required for store the arg. */
    LOGFW_FORCE_INLINE static std::size_t bytes_required(string_view value)
    {
        const std::size_t size = std::min< std::size_t >(value.size(), string_max_length);
        return size + 1;
    }

    /**
     * Copy type to buffer.
     * @return Used bytes.
     *
     * layout: [str-size][str-bytes]
     */
    LOGFW_FORCE_INLINE static std::size_t encode(string_view value, char* buffer) noexcept
    {
        const std::size_t size = std::min< std::size_t >(value.size(), string_max_length);
        buffer[0] = uint8_t(size);
        std::memcpy(buffer + 1, value.data(), size);
        return size + 1;
    }

    /**
     * Copy type from buffer.
     * @return Used bytes.
     */
    LOGFW_FORCE_INLINE static std::size_t decode(string_view& value, const char* buffer, std::size_t size)
    {
        if (LOGFW_UNLIKELY(size == 0)) {
            throw std::runtime_error("buffer too small");
        }

        const std::size_t bytes = size_t(buffer[0]);
        if (LOGFW_UNLIKELY(bytes + 1 > size)) {
            throw std::runtime_error("buffer too small");
        }

        value = string_view{buffer + 1, bytes};
        return bytes + 1;
    }
};


template<>
struct arg_io< const char* >
    : arg_io< string_view >
{};

template<>
struct arg_io< std::string >
    : arg_io< string_view >
{};

template< std::size_t N >
struct arg_io< char[N] >
{
    static_assert( N < 256, "" );

    /** Return maximum numbers of bytes to store the type in the buffer */
    static constexpr std::size_t max_bytes_required() noexcept
    {
        return N;
    }

    /** Return numbers of actual bytes required for store the arg */
    static constexpr std::size_t bytes_required(const char (&value)[N]) noexcept
    {
        return N;
    }

    /**
     * Copy type to buffer.
     * @return used bytes
     *
     * layout: [str-size][str-bytes]
     */
    LOGFW_FORCE_INLINE static std::size_t encode(const char (&value)[N], char* buffer) noexcept
    {
        buffer[0] = static_cast< uint8_t >(N - 1);
        std::memcpy(buffer + 1, value, N - 1);
        return N;
    }
};

template< class T >
struct arg_io< T, typename std::enable_if< std::is_pointer< T >::value >::type >
{
    /** Return maximum numbers of bytes to store the type in the buffer */
    static constexpr std::size_t max_bytes_required() noexcept
    {
        return sizeof(std::uintptr_t);
    }

    /** Return numbers of actual bytes required for store the arg */
    static constexpr std::size_t bytes_required(const T& value) noexcept
    {
        return sizeof(std::uintptr_t);
    }

    /**
     * Copy type to buffer.
     * @return used bytes
     */
    LOGFW_FORCE_INLINE static std::size_t encode(T value, char* buffer) noexcept
    {
        *reinterpret_cast< std::uintptr_t* >(buffer) = reinterpret_cast< std::uintptr_t >(value);
        return sizeof(std::uintptr_t);
    }

    /**
     * Copy type from buffer.
     * @return used bytes
     */
    LOGFW_FORCE_INLINE static std::size_t decode(T& value, const char* buffer, size_t size)
    {
        if (LOGFW_UNLIKELY(size < sizeof(std::uintptr_t))) {
            throw std::runtime_error("Buffer too small");
        }

        value = reinterpret_cast< const T& >(*reinterpret_cast< const std::uintptr_t* >(buffer));
        return sizeof(std::uintptr_t);
    }
};

template< class... Args >
struct encode_impl;

template<>
struct encode_impl<>
{
    /* Empty args */
    static constexpr std::size_t encode([[gnu::unused]] char* buffer) noexcept
    {
        return 0;
    }

    /* Required buffer size */
    static constexpr std::size_t max_bytes_required() noexcept
    {
        return 0;
    }

    /* Required buffer size */
    static constexpr std::size_t bytes_required() noexcept
    {
        return 0;
    }
};

template< class T >
struct encode_impl< T >
{
    LOGFW_FORCE_INLINE static std::size_t encode(const T& value, char* buffer)
    {
        return arg_io< T >::encode(value, buffer);
    }

    static constexpr std::size_t max_bytes_required()
    {
        return arg_io< T >::max_bytes_required();
    }

    LOGFW_FORCE_INLINE static std::size_t bytes_required(const T& value)
    {
        return arg_io< T >::max_bytes_required(value);
    }
};

template< class T, class... Args >
struct encode_impl< T, Args... >
{
    LOGFW_FORCE_INLINE static std::size_t encode(const T& value, const Args&... args, char* buffer)
    {
        /* Encode type */
        const std::size_t encoded_size = encode_impl< T >::encode(value, buffer);

        /* Encode rest args */
        return encoded_size + encode_impl< Args... >::encode(args..., buffer + encoded_size);
    }

    /* Required buffer size */
    static constexpr std::size_t max_bytes_required()
    {
        return encode_impl< T >::max_bytes_required() + encode_impl< Args... >::max_bytes_required();
    }

    LOGFW_FORCE_INLINE static std::size_t bytes_required(const T& value, const Args&... args)
    {
        return encode_impl< T >::bytes_required(value) + encode_impl< Args... >::bytes_required(args...);
    }
};

} /* namespace detail */
} /* namespace logfw */

#endif /* MADLIFE_encode_impl_021216225339_MADLIFE */
