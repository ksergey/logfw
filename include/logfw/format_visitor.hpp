/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_format_visitor_021216170704_MADLIFE
#define MADLIFE_format_visitor_021216170704_MADLIFE

#include <cctype>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "string_view.hpp"
#include "atomic_buffer.hpp"
#include "arg_io.hpp"

namespace logfw {

class format_visitor final
{
private:
    /* format string */
    const string_view format_;

    /* args buffer */
    const char* buffer_;

    /* args buffer size */
    const std::size_t size_;

    /* args buffer offset */
    std::size_t offset_{0};

    /* last apply error */
    const char* last_error_{nullptr};

public:
    format_visitor(const format_visitor&) = delete;
    format_visitor& operator=(const format_visitor&) = delete;

    format_visitor(string_view format, const char* buffer, std::size_t size)
        : format_(format)
        , buffer_(buffer)
        , size_(size)
    {}

    bool apply(std::ostream& os)
    {
        /* clear arg offset */
        offset_ = 0;
        /* clear last error */
        last_error_ = nullptr;

        for (std::size_t i = 0; i < format_.size(); ++i) {
            if (format_[i] == '}') {
                if (__unlikely(!next_is(i, '}'))) {
                    last_error_ = "format error, close brace without open brace";
                    return false;
                }
                os << '}';
                ++i;
            } else if (format_[i] == '{') {
                if (next_is(i, '{')) {
                    os << '{';
                    ++i;
                } else {
                    auto found = format_.find('}', i + 1);
                    if (__unlikely(found == std::string::npos)) {
                        last_error_ = "format error: close brace not found";
                        return false;
                    }

                    auto result = print_arg(os, format_.substr(i + 1, found - i - 1));
                    if (__unlikely(result == std::string::npos)) {
                        /* last error filled inside print_arg */
                        return false;
                    }

                    i = found;
                }
            } else {
                os << format_[i];
            }
        }

        return true;
    }

private:
    __force_inline bool next_is(std::size_t i, char ch) const
    {
        return (i + 1 < format_.size()) && (format_[i + 1] == ch);
    }

    __force_inline std::size_t print_arg(std::ostream& os, const string_view& spec)
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

        std::size_t result =
            write_if< int8_t >(os, type, flags) ||
            write_if< uint8_t >(os, type, flags) ||
            write_if< int16_t >(os, type, flags) ||
            write_if< uint16_t >(os, type, flags) ||
            write_if< int32_t >(os, type, flags) ||
            write_if< uint32_t >(os, type, flags) ||
            write_if< int64_t >(os, type, flags) ||
            write_if< uint64_t >(os, type, flags) ||
            write_if< char >(os, type, flags) ||
            write_if< double >(os, type, flags) ||
            write_if< float >(os, type, flags) ||
            write_if< string_view >(os, type, flags);

        if (__unlikely(result == 0)) {
            last_error_ = "unknown argument type";
            return decode_failed;
        }

        if (__unlikely(result == decode_failed)) {
            std::cerr << "decode failed \n";
        }

        return result;
    }

    template< class T >
    __force_inline std::size_t write_if(std::ostream& os, const string_view& type,
            const string_view& flags)
    {
        if (!type_format_is< T >(type)) {
            return 0;
        }

        /* decode type */
        T value;
        const std::size_t size = arg_io< T >::decode(value, buffer_ + offset_, size_ - offset_);

        if (__likely(size != decode_failed)) {
            /* save ostream flags */
            std::ios state{nullptr};
            state.copyfmt(os);

            /* apply formating flags to ostream */
            apply_format_flags(os, flags);

            /* write value */
            os << value;

            /* restore ostream formating flags */
            os.copyfmt(state);

            offset_ += size;
        }

        return size;
    }

    __force_inline void apply_format_flags(std::ostream& os, const string_view& flags) const
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
                throw std::runtime_error("bad format specifier (width)");
            }
        }
        os.width(width);

        std::size_t precision{0};
        while (idx < flags.size()) {
            if (std::isdigit(flags[idx])) {
                precision = precision * 10 + (flags[idx] - '0');
                ++idx;
            } else {
                throw std::runtime_error("bad format specifier (precision)");
            }
        }
        os << std::setprecision(precision);
    }
};

} /* namespace logfw */

#endif /* MADLIFE_format_visitor_021216170704_MADLIFE */
