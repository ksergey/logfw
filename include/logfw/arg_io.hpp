/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_arg_io_291116174409_MADLIFE
#define MADLIFE_arg_io_291116174409_MADLIFE

#include <cstring>
#include <type_traits>
#include <algorithm>
#include <string>
#include <limits>
#include "string_view.hpp"

namespace logfw {

/** failed decode result */
static constexpr const std::size_t decode_failed = std::string::npos;

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
    __force_inline static constexpr std::size_t max_bytes_required()
    {
        return sizeof(T);
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    __force_inline static constexpr std::size_t bytes_required(__unused const T& value)
    {
        return sizeof(T);
    }

    /**
     * copy type to buffer.
     * return bytes used
     */
    static __force_inline std::size_t encode(const T& value, char* buffer)
    {
        *reinterpret_cast< T* >(buffer) = value;
        return sizeof(T);
    }

    /**
     * copy type from buffer.
     * return bytes used
     */
    static __force_inline std::size_t decode(T& value, const char* buffer, size_t size)
    {
        if (__unlikely(size < sizeof(T))) {
            return decode_failed;
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
    __force_inline static constexpr std::size_t max_bytes_required()
    {
        return string_max_length + 1;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    __force_inline static std::size_t bytes_required(const char* value)
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
    static __force_inline std::size_t encode(const char* value, char* buffer)
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
    __force_inline static constexpr std::size_t max_bytes_required()
    {
        return string_max_length + 1;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    __force_inline static std::size_t bytes_required(const std::string& value)
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
    static __force_inline std::size_t encode(const std::string& value, char* buffer)
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
    __force_inline static constexpr std::size_t max_bytes_required()
    {
        return string_max_length + 1;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    __force_inline static std::size_t bytes_required(const string_view& value)
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
    static __force_inline std::size_t encode(const string_view& value, char* buffer)
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
    static __force_inline std::size_t decode(string_view& value,
            const char* buffer, std::size_t size)
    {
        if (__unlikely(size == 0)) {
            return decode_failed;
        }

        const std::size_t bytes = size_t(buffer[0]);
        if (__unlikely(bytes + 1 < size)) {
            return decode_failed;
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
    __force_inline static constexpr std::size_t max_bytes_required()
    {
        return N;
    }

    /**
     * return numbers of actual bytes required for store the arg
     */
    __force_inline static constexpr std::size_t bytes_required(__unused const char (&value)[N])
    {
        return N;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static __force_inline std::size_t encode(const char (&value)[N], char* buffer)
    {
        buffer[0] = uint8_t(N - 1);
        std::memcpy(buffer + 1, value, N - 1);
        return N;
    }
};

} /* namespace logfw */

#endif /* MADLIFE_arg_io_291116174409_MADLIFE */
