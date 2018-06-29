// ------------------------------------------------------------
// Copyright (c) 2016-2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef MADLIFE_write_impl_031216001659_MADLIFE
#define MADLIFE_write_impl_031216001659_MADLIFE

#include <iomanip>
#include "../decoder.hpp"

namespace logfw::detail {

template< char Ch >
constexpr bool next_is(std::string_view str, std::size_t index)
{
    ++index;
    return index < str.size() ? str[index] == Ch : false;
}

LOGFW_FORCE_INLINE void apply_format_flags(std::ostream& os, std::string_view flags)
{
    std::size_t idx = 0;
    while (idx < flags.size()) {
        if (flags[idx] == '0') {
            os.fill('0');
        } else if (flags[idx] == '-') {
            os << std::left;
        } else if (flags[idx] == '+') {
            os << std::showpos;
        } else if (flags[idx] == 'x') {
            os << std::hex;
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

    std::size_t precision{0};
    while (idx < flags.size()) {
        if (std::isdigit(flags[idx])) {
            precision = precision * 10 + (flags[idx] - '0');
            ++idx;
        } else {
            /* What to do? */
            return ;
        }
    }

    if (width > 0) {
        os << std::setw(width);
    }

    if (precision > 0) {
        os << std::fixed << std::setprecision(precision);
    }
}


template< class T >
struct write_if_match_impl
{
    LOGFW_FORCE_INLINE static bool run(std::ostream& os, std::string_view type, std::string_view flags, decoder& d)
    {
        if (!d.is< T >(type)) {
            return false;
        }

        /* Decode value */
        T value;
        d.decode(value);

        /* Save ostream flags */
        std::ios state{nullptr};
        state.copyfmt(os);

        /* Apply formating flags to ostream */
        apply_format_flags(os, flags);

        /* Write value */
        os << value;

        /* Restore ostream formating flags */
        os.copyfmt(state);

        return true;
    }
};

template< class T >
struct write_if_match_impl< T* >
{
    LOGFW_FORCE_INLINE static bool run(std::ostream& os, std::string_view type, std::string_view flags, decoder& d)
    {
        if (!d.is< T* >(type)) {
            return false;
        }

        /* Decode value */
        void* value;
        d.decode(value);

        /* Save ostream flags */
        std::ios state{nullptr};
        state.copyfmt(os);

        /* Apply formating flags to ostream */
        apply_format_flags(os, flags);

        /* Write value */
        os << value;

        /* Restore ostream formating flags */
        os.copyfmt(state);

        return true;
    }
};

template< class T >
LOGFW_FORCE_INLINE bool write_if_match(std::ostream& os, std::string_view type, std::string_view flags, decoder& d)
{
    return write_if_match_impl< T >::run(os, type, flags, d);
}

LOGFW_FORCE_INLINE void write_arg(std::ostream& os, std::string_view spec, decoder& d)
{
    /* find type:spec delimiter */
    auto found = spec.find(':');

    /* string representation of argument type */
    auto type = spec.substr(0, found);

    /* argument formating flags */
    std::string_view flags;
    if (found != std::string_view::npos) {
        flags = spec.substr(found + 1);
    }

    bool printed =
        write_if_match< std::int8_t >(os, type, flags, d) ||
        write_if_match< std::uint8_t >(os, type, flags, d) ||
        write_if_match< std::int16_t >(os, type, flags, d) ||
        write_if_match< std::uint16_t >(os, type, flags, d) ||
        write_if_match< std::int32_t >(os, type, flags, d) ||
        write_if_match< std::uint32_t >(os, type, flags, d) ||
        write_if_match< std::int64_t >(os, type, flags, d) ||
        write_if_match< std::uint64_t >(os, type, flags, d) ||
        write_if_match< char >(os, type, flags, d) ||
        write_if_match< double >(os, type, flags, d) ||
        write_if_match< float >(os, type, flags, d) ||
        write_if_match< std::string_view >(os, type, flags, d) ||
        write_if_match< void* >(os, type, flags, d);

    if (LOGFW_UNLIKELY(!printed)) {
        throw std::runtime_error("Unknown format type");
    }
}

} // namespace logfw::detail

#endif /* MADLIFE_write_impl_031216001659_MADLIFE */
