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
    static __force_inline void encode(atomic_buffer& buffer, std::size_t offset = 0)
    {
        buffer.capacity(offset);
    }

    /* required buffer size */
    static __force_inline constexpr std::size_t max_bytes_required()
    {
        return 0;
    }
};

template< class T >
struct encode_impl< T >
{
    static __force_inline void encode(const T& value, atomic_buffer& buffer, std::size_t offset = 0)
    {
        /* encode type */
        const std::size_t encoded_size = arg_io< T >::encode(value, buffer, offset);
        /* update buffer capacity */
        buffer.capacity(encoded_size + offset);
    }

    /* required buffer size */
    static __force_inline constexpr std::size_t max_bytes_required()
    {
        return arg_io< T >::max_bytes_required();
    }
};

template< class T, class... Args >
struct encode_impl< T, Args... >
{
    static __force_inline void encode(const T& value, const Args&... args,
            atomic_buffer& buffer, std::size_t offset = 0)
    {
        /* encode type */
        const std::size_t encoded_size = arg_io< T >::encode(value, buffer, offset);
        /* encode rest args */
        encode_impl< Args... >::encode(args..., buffer, encoded_size + offset);
    }

    /* required buffer size */
    static __force_inline constexpr std::size_t max_bytes_required()
    {
        return arg_io< T >::max_bytes_required() + encode_impl< Args... >::max_bytes_required();
    }
};

} /* namespace detail */

/* encode args into buffer */
template< class... Args >
__force_inline void encode(atomic_buffer& buffer, const Args&... args)
{
    return detail::encode_impl< Args... >::encode(args..., buffer);
}

/* calculate max buffer size for encoding args */
template< class... Args >
__force_inline constexpr std::size_t max_bytes_required()
{
    return detail::encode_impl< Args... >::max_bytes_required();
}

} /* namespace logfw */

#endif /* MADLIFE_encoder_291116183743_MADLIFE */
