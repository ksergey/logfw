/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_write_031216001732_MADLIFE
#define MADLIFE_write_031216001732_MADLIFE

#include <iostream>
#include "detail/write_impl.hpp"
#include "decoder.hpp"

namespace logfw {

/**
 * serialize format string into ostream
 */
__force_inline void write(std::ostream& os, string_view fmt, const char* buffer, size_t size)
{
    /* argument decoder */
    decoder dec{buffer, size};

    for (std::size_t index = 0; index < fmt.size(); ++index) {
        char ch = fmt[index];

        if (__unlikely(ch == '{')) {

            if (detail::next_is< '{' >(fmt, index)) {
                ++index;
                os << ch;
            } else {
                /* find close brace */
                auto found = fmt.find('}', index + 1);
                if (__unlikely(found == std::string::npos)) {
                    throw std::runtime_error("format error (close brace not found)");
                }

                /* print arg */
                detail::write_arg(os, fmt.substr(index + 1, found - index - 1), dec);

                /* skip specifier */
                index = found;
            }

        } else if (__unlikely(ch == '}')) {

            if (detail::next_is< '}' >(fmt, index)) {
                ++index;
                os << ch;
            } else {
                throw std::runtime_error("format error (unexpected close brace)");
            }

        } else {
            os << ch;
        }
    }
}

} /* namespace logfw */

#endif /* MADLIFE_write_031216001732_MADLIFE */
