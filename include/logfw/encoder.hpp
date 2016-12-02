/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_encoder_291116183743_MADLIFE
#define MADLIFE_encoder_291116183743_MADLIFE

#include "detail/encode_impl.hpp"

namespace logfw {

/** compile-time args encoder */
struct encoder
{
    /**
     * encode args into buffer
     * @return bytes used
     */
    template< class... Args >
    __force_inline static std::size_t encode(char* buffer, const Args&... args)
    {
        return detail::encode_impl< Args... >::encode(args..., buffer);
    }

    /**
     * calculate max buffer size for encoding args
     * @return max size of buffer
     */
    template< class... Args >
    __force_inline static constexpr std::size_t max_bytes_required()
    {
        return detail::encode_impl< Args... >::max_bytes_required();
    }

    /**
     * calculate actual buffer size for encoding args
     * @return size of buffer
     */
    template< class... Args >
    __force_inline static constexpr std::size_t bytes_required(const Args&... args)
    {
        return detail::encode_impl< Args... >::bytes_required(args...);
    }
};

} /* namespace logfw */

#endif /* MADLIFE_encoder_291116183743_MADLIFE */
