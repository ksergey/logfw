/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_encoder_291116183743_MADLIFE
#define MADLIFE_encoder_291116183743_MADLIFE

#include "arg_io.hpp"

namespace logfw {
namespace detail {

template< class... Args >
struct encode_impl;

template<>
struct encode_impl<>
{
    /* empty args */
    static __force_inline std::size_t encode(__unused char* buffer)
    {
        return 0;
    }

    /* required buffer size */
    static __force_inline constexpr std::size_t max_bytes_required()
    {
        return 0;
    }

    /* required buffer size */
    static __force_inline constexpr std::size_t bytes_required()
    {
        return 0;
    }
};

template< class T >
struct encode_impl< T >
{
    static __force_inline std::size_t encode(const T& value, char* buffer)
    {
        return arg_io< T >::encode(value, buffer);
    }

    static __force_inline constexpr std::size_t max_bytes_required()
    {
        return arg_io< T >::max_bytes_required();
    }

    static __force_inline std::size_t bytes_required(const T& value)
    {
        return arg_io< T >::max_bytes_required(value);
    }
};

template< class T, class... Args >
struct encode_impl< T, Args... >
{
    static __force_inline std::size_t encode(const T& value,
            const Args&... args, char* buffer)
    {
        /* encode type */
        const std::size_t encoded_size = encode_impl< T >::encode(value, buffer);

        /* encode rest args */
        return encoded_size + encode_impl< Args... >::encode(args..., buffer + encoded_size);
    }

    /* required buffer size */
    static __force_inline constexpr std::size_t max_bytes_required()
    {
        return encode_impl< T >::max_bytes_required() + encode_impl< Args... >::max_bytes_required();
    }

    static __force_inline std::size_t bytes_required(const T& value, const Args&... args)
    {
        return encode_impl< T >::bytes_required(value) + encode_impl< Args... >::bytes_required(args...);
    }
};

} /* namespace detail */

/* encode args into buffer */
template< class... Args >
__force_inline std::size_t encode(char* buffer, const Args&... args)
{
    return detail::encode_impl< Args... >::encode(args..., buffer);
}

/* calculate max buffer size for encoding args */
template< class... Args >
__force_inline constexpr std::size_t max_bytes_required()
{
    return detail::encode_impl< Args... >::max_bytes_required();
}

/* calculate actual buffer size for encoding args */
template< class... Args >
__force_inline constexpr std::size_t bytes_required(const Args&... args)
{
    return detail::encode_impl< Args... >::bytes_required(args...);
}

} /* namespace logfw */

#endif /* MADLIFE_encoder_291116183743_MADLIFE */
