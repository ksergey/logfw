/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include "logfw/make_format.hpp"
#include "logfw/arg_io.hpp"
#include "logfw/type_string.hpp"
#include "logfw/encoder.hpp"
#include "logfw/fmt.hpp"

using namespace logfw;

template< class String, std::size_t N, class... Args >
inline std::size_t log_impl(char (&buffer)[N], const Args&... args)
{
    using format = make_format< String, Args... >;
    /* encode with format */
    std::size_t bytes = encode< Args... >(buffer, args...);
    /* encode without format */
    //encode< Args... >(buffer, args...);

    static constexpr size_t max_buffer_size = max_bytes_required< Args... >() + format::size() + 1;
    static_assert( max_buffer_size < 2048, "" );

    std::cout << "format: \"" << format::data() << "\"\n";
    std::cout << "encoded-size: \"" << bytes << "\"\n";
    std::cout << "max-buffer-size: \"" << (max_bytes_required< Args... >()) << "\"\n";
    std::cout << "max-buffer-size (with fmt): \"" << max_buffer_size << "\"\n";

    format_visitor fmt(format::str(), buffer, bytes);

    std::cout << "formatting result: \"" << fmt << "\"\n";

    return bytes;
}

#define log(buffer, fmt, ...) log_impl< typestring_is(fmt) >(buffer, ##__VA_ARGS__)

int main(__unused int argc, __unused char* argv[])
{
    char buffer[512];
    const char* test = "Paratruper";

    log(buffer, "hello {} {}", 123, "world");
    log(buffer, "test!");
    log(buffer, "test! {}", 123);
    log(buffer, "yahoo! {}", 9999ul);
    log(buffer, "yahoo! {} {}", 9999l, test);

    return 0;
}
