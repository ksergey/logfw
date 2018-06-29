// ------------------------------------------------------------
// Copyright (c) 2016-2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef MADLIFE_decoder_021216230550_MADLIFE
#define MADLIFE_decoder_021216230550_MADLIFE

#include <cassert>

#include "details/meta.hpp"
#include "details/encode_impl.hpp"
#include "details/type_format.hpp"

namespace logfw {

/** Runtime argument decoder */
class decoder
{
private:
    /* pointer to args buffer */
    const char* buffer_;

    /* size of buffer */
    std::size_t size_;

public:
    decoder(const char* buffer, std::size_t size)
        : buffer_(buffer)
        , size_(size)
    {}

    /** Decode type */
    template< class T >
    LOGFW_FORCE_INLINE void decode(T& value)
    {
        const std::size_t used = details::arg_io< T >::decode(value, buffer_, size_);

        assert( used <= size_ );

        buffer_ += used;
        size_ -= used;
    }

    /** Runtime type matching */
    template< class T >
    static bool is(std::string_view str)
    {
        using raw_type = details::clear_type< T >;
        using fmt = details::stringify< typename details::type_format< raw_type >::type >;
        static std::string_view format(fmt::data(), fmt::size());
        return format == str;
    }
};

} /* namespace logfw */

#endif /* MADLIFE_decoder_021216230550_MADLIFE */
