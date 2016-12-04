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
 * maximum length of loggable string.
 * strings will be trimed in case of greater 256 chars
 */
static constexpr const std::size_t string_max_length = std::numeric_limits< uint8_t >::max();

/** argument i/o handlers */
template< class T, class Enable = void >
struct arg_io;

template< class T >
struct arg_io< T, typename std::enable_if< std::is_arithmetic< T >::value >::type >
{
    /**
     * return maximum numbers of bytes to store the type in the buffer
     */
    static constexpr std::size_t max_bytes_required()
    {
        return sizeof(T);
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    static constexpr std::size_t bytes_required(__unused const T& value)
    {
        return sizeof(T);
    }

    /**
     * copy type to buffer.
     * return bytes used
     */
    static std::size_t encode(const T& value, char* buffer)
    {
        *reinterpret_cast< T* >(buffer) = value;
        return sizeof(T);
    }

    /**
     * copy type from buffer.
     * return bytes used
     */
    static std::size_t decode(T& value, const char* buffer, size_t size)
    {
        if (__unlikely(size < sizeof(T))) {
            throw std::runtime_error("buffer too small");
        }
        value = *reinterpret_cast< const T* >(buffer);
        return sizeof(T);
    }
};

template<>
struct arg_io< const char* >
{
    /**
     * return maximum numbers of bytes to store the type in the buffer
     */
    static constexpr std::size_t max_bytes_required()
    {
        return string_max_length + 1;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    static std::size_t bytes_required(const char* value)
    {
        const std::size_t size = std::min< std::size_t >(std::strlen(value), string_max_length);
        return size + 1;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static std::size_t encode(const char* value, char* buffer)
    {
        const std::size_t size = std::min< std::size_t >(std::strlen(value), string_max_length);
        buffer[0] = uint8_t(size);
        std::memcpy(buffer + 1, value, size);
        return size + 1;
    }
};

template<>
struct arg_io< std::string >
{
    /**
     * return maximum numbers of bytes to store the type in the buffer
     */
    static constexpr std::size_t max_bytes_required()
    {
        return string_max_length + 1;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    static std::size_t bytes_required(const std::string& value)
    {
        const std::size_t size = std::min< std::size_t >(value.size(), string_max_length);
        return size + 1;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static std::size_t encode(const std::string& value, char* buffer)
    {
        const std::size_t size = std::min< std::size_t >(value.size(), string_max_length);
        buffer[0] = uint8_t(size);
        std::memcpy(buffer + 1, value.data(), size);
        return size + 1;
    }
};

template<>
struct arg_io< string_view >
{
    /**
     * return maximum numbers of bytes to store the type in the buffer
     */
    static constexpr std::size_t max_bytes_required()
    {
        return string_max_length + 1;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    static std::size_t bytes_required(string_view value)
    {
        const std::size_t size = std::min< std::size_t >(value.size(), string_max_length);
        return size + 1;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static std::size_t encode(string_view value, char* buffer)
    {
        const std::size_t size = std::min< std::size_t >(value.size(), string_max_length);
        buffer[0] = uint8_t(size);
        std::memcpy(buffer + 1, value.data(), size);
        return size + 1;
    }

    /**
     * copy type from buffer.
     * return bytes used
     */
    static std::size_t decode(string_view& value,
            const char* buffer, std::size_t size)
    {
        if (__unlikely(size == 0)) {
            throw std::runtime_error("buffer too small");
        }

        const std::size_t bytes = size_t(buffer[0]);
        if (__unlikely(bytes + 1 > size)) {
            throw std::runtime_error("buffer too small");
        }

        value = string_view{buffer + 1, bytes};
        return bytes + 1;

    }
};

template< std::size_t N >
struct arg_io< char[N] >
{
    static_assert( N < 256, "" );

    /**
     * return maximum numbers of bytes to store the type in the buffer
     */
    static constexpr std::size_t max_bytes_required()
    {
        return N;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    static constexpr std::size_t bytes_required(__unused const char (&value)[N])
    {
        return N;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static std::size_t encode(const char (&value)[N], char* buffer)
    {
        buffer[0] = uint8_t(N - 1);
        std::memcpy(buffer + 1, value, N - 1);
        return N;
    }
};

template< class... Args >
struct encode_impl;

template<>
struct encode_impl<>
{
    /* empty args */
    static std::size_t encode(__unused char* buffer)
    {
        return 0;
    }

    /* required buffer size */
    static constexpr std::size_t max_bytes_required()
    {
        return 0;
    }

    /* required buffer size */
    static constexpr std::size_t bytes_required()
    {
        return 0;
    }
};

template< class T >
struct encode_impl< T >
{
    static std::size_t encode(const T& value, char* buffer)
    {
        return arg_io< T >::encode(value, buffer);
    }

    static constexpr std::size_t max_bytes_required()
    {
        return arg_io< T >::max_bytes_required();
    }

    static std::size_t bytes_required(const T& value)
    {
        return arg_io< T >::max_bytes_required(value);
    }
};

template< class T, class... Args >
struct encode_impl< T, Args... >
{
    static std::size_t encode(const T& value,
            const Args&... args, char* buffer)
    {
        /* encode type */
        const std::size_t encoded_size = encode_impl< T >::encode(value, buffer);

        /* encode rest args */
        return encoded_size + encode_impl< Args... >::encode(args..., buffer + encoded_size);
    }

    /* required buffer size */
    static constexpr std::size_t max_bytes_required()
    {
        return encode_impl< T >::max_bytes_required() + encode_impl< Args... >::max_bytes_required();
    }

    static std::size_t bytes_required(const T& value, const Args&... args)
    {
        return encode_impl< T >::bytes_required(value) + encode_impl< Args... >::bytes_required(args...);
    }
};

} /* namespace detail */
} /* namespace logfw */

#endif /* MADLIFE_encode_impl_021216225339_MADLIFE */
