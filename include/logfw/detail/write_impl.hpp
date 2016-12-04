/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_write_impl_031216001659_MADLIFE
#define MADLIFE_write_impl_031216001659_MADLIFE

#include <iomanip>
#include "../decoder.hpp"
#include "../common.hpp"

namespace logfw {
namespace detail {

template< char Ch >
__force_inline bool next_is(const string_view& str, std::size_t index)
{
    ++index;
    return index < str.size()
        ? str[index] == Ch
        : false;
}

__force_inline void apply_format_flags(std::ostream& os, string_view flags)
{
    std::size_t idx = 0;
    while (idx < flags.size()) {
        if (flags[idx] == '0') {
            os.fill('0');
        } else if (flags[idx] == '-') {
            os << std::left;
        } else if (flags[idx] == '+') {
            os << std::showpos;
        } else {
            break;
        }
        ++idx;
    }

    std::size_t width{0};
    while (idx < flags.size()) {
        if (std::isdigit(flags[idx])) {
            width = width * 10 + (flags[idx] - '0');
            ++idx;
        } else if (flags[idx] == '.') {
            ++idx;
            break;
        } else {
            /* what to do? */
            return ;
        }
    }
    os.width(width);

    std::size_t precision{0};
    while (idx < flags.size()) {
        if (std::isdigit(flags[idx])) {
            precision = precision * 10 + (flags[idx] - '0');
            ++idx;
        } else {
            /* what to do? */
            return ;
        }
    }
    os << std::setprecision(precision);
}

template< class T >
__force_inline bool write_if_match(std::ostream& os,
        string_view type, string_view flags, decoder& d)
{
    if (!d.is< T >(type)) {
        return false;
    }

    /* decode value */
    T value;
    d.decode(value);

    /* save ostream flags */
    std::ios state{nullptr};
    state.copyfmt(os);

    /* apply formating flags to ostream */
    apply_format_flags(os, flags);

    /* write value */
    os << value;

    /* restore ostream formating flags */
    os.copyfmt(state);


    return true;
}

__force_inline void write_arg(std::ostream& os, string_view spec, decoder& d)
{
    /* find type:spec delimiter */
    auto found = spec.find(':');

    /* string representation of argument type */
    string_view type = spec.substr(0, found);

    /* argument formating flags */
    string_view flags;
    if (found != std::string::npos) {
        flags = spec.substr(found + 1);
    }

    bool printed =
        write_if_match< int8_t >(os, type, flags, d) ||
        write_if_match< uint8_t >(os, type, flags, d) ||
        write_if_match< int16_t >(os, type, flags, d) ||
        write_if_match< uint16_t >(os, type, flags, d) ||
        write_if_match< int32_t >(os, type, flags, d) ||
        write_if_match< uint32_t >(os, type, flags, d) ||
        write_if_match< int64_t >(os, type, flags, d) ||
        write_if_match< uint64_t >(os, type, flags, d) ||
        write_if_match< char >(os, type, flags, d) ||
        write_if_match< double >(os, type, flags, d) ||
        write_if_match< float >(os, type, flags, d) ||
        write_if_match< string_view >(os, type, flags, d);

    if (__unlikely(!printed)) {
        throw std::runtime_error("unknown format type");
    }
}

} /* namespace detail */
} /* namespace logfw */

#endif /* MADLIFE_write_impl_031216001659_MADLIFE */
