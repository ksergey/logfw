/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_print_291116234914_MADLIFE
#define MADLIFE_print_291116234914_MADLIFE

#include <cctype>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "string_view.hpp"
#include "atomic_buffer.hpp"
#include "arg_io.hpp"

namespace logfw {
namespace detail {

class ios_saver final
{
private:
    std::ostream& os_;
    std::ios state_{nullptr};

public:
    __force_inline explicit ios_saver(std::ostream& os)
        : os_(os)
    {
        state_.copyfmt(os);
    }

    __force_inline ~ios_saver()
    {
        os_.copyfmt(state_);
    }
};

__force_inline void apply_format_flags(std::ostream& os, const string_view& flags)
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

__force_inline std::size_t print_arg(std::ostream& os, const string_view& spec_type,
        const string_view& spec_flags, const atomic_buffer& buffer, std::size_t offset = 0)
{
    std::size_t result{0};
    ios_saver saver{os};
    apply_format_flags(os, spec_flags);

    if (type_format_is< int8_t >(spec_type)) {
        int8_t arg;
        result = arg_io< int8_t >::decode(arg, buffer, offset);
        os << int(arg);
    } else if (type_format_is< uint8_t >(spec_type)) {
        uint8_t arg;
        result = arg_io< uint8_t >::decode(arg, buffer, offset);
        os << unsigned(arg);
    } else if (type_format_is< int16_t >(spec_type)) {
        int16_t arg;
        result = arg_io< int16_t >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< uint16_t >(spec_type)) {
        uint16_t arg;
        result = arg_io< uint16_t >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< int32_t >(spec_type)) {
        int32_t arg;
        result = arg_io< int32_t >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< uint32_t >(spec_type)) {
        uint32_t arg;
        result = arg_io< uint32_t >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< int64_t >(spec_type)) {
        int64_t arg;
        result = arg_io< int64_t >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< uint64_t >(spec_type)) {
        uint64_t arg;
        result = arg_io< uint64_t >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< string_view >(spec_type)) {
        string_view arg;
        result = arg_io< string_view >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< char >(spec_type)) {
        char arg;
        result = arg_io< char >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< double >(spec_type)) {
        double arg;
        result = arg_io< double >::decode(arg, buffer, offset);
        os << arg;
    } else if (type_format_is< float >(spec_type)) {
        float arg;
        result = arg_io< float >::decode(arg, buffer, offset);
        os << arg;
    } else {
        throw std::runtime_error("unknown format spec \"" + spec_type.to_string() + "\"");
    }

    return result;
}

} /* namespace detail */

/**
 * print according to format
 */
__force_inline void print(std::ostream& os, const string_view& format,
        const atomic_buffer& buffer, std::size_t offset = 0)
{
    for (std::size_t i = 0; i < format.size(); ++i) {
        auto ch = format[i];
        switch (ch) {
            case '{':
                {
                    if (i + 1 < format.size()) {
                        auto next_ch = format[i + 1];
                        if (next_ch == '{') {
                            os << '{';
                            ++i;
                        } else {
                            auto close_brace_found = format.find('}', i + 1);
                            if (__unlikely(close_brace_found == std::string::npos)) {
                                throw std::runtime_error("format error (close brace not found)");
                            }

                            string_view spec(format.substr(i + 1, close_brace_found - i - 1));
                            auto delimiter_found = spec.find(':');
                            string_view spec_type = spec.substr(0, delimiter_found);
                            string_view spec_flags = delimiter_found == std::string::npos
                                ? string_view{}
                                : spec.substr(delimiter_found + 1);

                            /* print argument */
                            offset += detail::print_arg(os, spec_type, spec_flags, buffer, offset);

                            /* skip format */
                            i += spec.size() + 1;
                        }
                    } else {
                        throw std::runtime_error("format error (unexpected end of format)");
                    }
                }
                break;
            case '}':
                {
                    if (__likely(i + 1 < format.size())) {
                        auto next_ch = format[i + 1];
                        if (__unlikely(next_ch != '}')) {
                            throw std::runtime_error("format error (found close brace)");
                        }
                    } else {
                        throw std::runtime_error("format error (found close brace)");
                    }
                    os << '}';
                    ++i;
                }
                break;
            default:
                {
                    os << ch;
                }
                break;
        }
    }

    os << '\n';
}

/**
 * extract format from buffer and print according to format
 */
__force_inline void print(std::ostream& os, const atomic_buffer& buffer)
{
    string_view format;
    std::size_t offset = arg_io< string_view >::decode(format, buffer, 0);
    print(os, format, buffer, offset);
}

} /* namespace logfw */

#endif /* MADLIFE_print_291116234914_MADLIFE */
