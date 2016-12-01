/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_arg_io_291116174409_MADLIFE
#define MADLIFE_arg_io_291116174409_MADLIFE

#include <cstring>
#include <type_traits>
#include <algorithm>
#include <limits>
#include "atomic_buffer.hpp"
#include "string_view.hpp"

namespace logfw {

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
     * copy type to buffer.
     * return bytes used
     */
    static __force_inline std::size_t encode(const T& value, atomic_buffer& buffer,
            std::size_t offset = 0)
    {
        buffer.as< T >(offset) = value;
        return sizeof(T);
    }

    /**
     * copy type from buffer.
     * return bytes used
     */
    static __force_inline std::size_t decode(T& value, const atomic_buffer& buffer,
            std::size_t offset = 0)
    {
        value = buffer.as< T >(offset);
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
        return std::numeric_limits< uint8_t >::max() + 1;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static __force_inline std::size_t encode(const char* value, atomic_buffer& buffer,
            std::size_t offset = 0)
    {
        const std::size_t size = std::min< std::size_t >(std::strlen(value), std::numeric_limits< uint8_t >::max());
        buffer.as< uint8_t >(offset) = size;
        std::memcpy(buffer.data(offset + 1), value, size);
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
        return std::numeric_limits< uint8_t >::max() + 1;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static __force_inline std::size_t encode(const std::string& value, atomic_buffer& buffer,
            std::size_t offset = 0)
    {
        const std::size_t size = std::min< std::size_t >(value.size(), std::numeric_limits< uint8_t >::max());
        buffer.as< uint8_t >(offset) = size;
        std::memcpy(buffer.data(offset + 1), value.data(), size);
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
        return std::numeric_limits< uint8_t >::max() + 1;
    }

    /**
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static __force_inline std::size_t encode(const string_view& value, atomic_buffer& buffer,
            std::size_t offset = 0)
    {
        const std::size_t size = std::min< std::size_t >(value.size(), std::numeric_limits< uint8_t >::max());
        buffer.as< uint8_t >(offset) = size;
        std::memcpy(buffer.data(offset + 1), value.data(), size);
        return size + 1;
    }

    /**
     * copy type from buffer.
     * return bytes used
     */
    static __force_inline std::size_t decode(string_view& value, const atomic_buffer& buffer,
            std::size_t offset = 0)
    {
        const std::size_t size = buffer.as< uint8_t >(offset);
        value = string_view{reinterpret_cast< const char* >(buffer.data(offset + 1)), size};
        return size + 1;
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
     * copy type to buffer.
     * return bytes used
     *
     * layout: [str-size][str-bytes]
     */
    static __force_inline std::size_t encode(const char (&value)[N], atomic_buffer& buffer,
            std::size_t offset = 0)
    {
        buffer.as< uint8_t >(offset) = N - 1;
        std::memcpy(buffer.data(offset + 1), value, N - 1);
        return N;
    }
};

} /* namespace logfw */

#endif /* MADLIFE_arg_io_291116174409_MADLIFE */
