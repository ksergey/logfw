// ------------------------------------------------------------
// Copyright (c) 2016-2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_write_290618133356
#define KSERGEY_write_290618133356

#include <iostream>

#include "compiler.hpp"
#include "decoder.hpp"
#include "detail/write_impl.hpp"

namespace logfw {

/// Serialize format string into ostream
LOGFW_FORCE_INLINE void write(std::ostream& os, std::string_view fmt, const char* buffer, size_t size)
{
    /* argument decoder */
    decoder dec{buffer, size};

    for (std::size_t index = 0; index < fmt.size(); ++index) {
        char ch = fmt[index];

        if (ch == '{') {

            if (detail::next_is< '{' >(fmt, index)) {
                ++index;
                os << ch;
            } else {
                /* find close brace */
                auto found = fmt.find('}', index + 1);
                if (LOGFW_UNLIKELY(found == std::string_view::npos)) {
                    throw std::runtime_error("format error (close brace not found)");
                }

                /* print arg */
                detail::write_arg(os, fmt.substr(index + 1, found - index - 1), dec);

                /* skip specifier */
                index = found;
            }

        } else if (ch == '}') {

            if (LOGFW_LIKELY(detail::next_is< '}' >(fmt, index))) {
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

#endif /* KSERGEY_write_290618133356 */
