/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include "logfw/atomic_buffer.hpp"
#include "logfw/make_format.hpp"
#include "logfw/arg_io.hpp"
#include "logfw/type_string.hpp"
#include "logfw/encoder.hpp"

using namespace logfw;

template< class String, std::size_t N, class... Args >
inline std::size_t log_impl(char (&dest)[N], const Args&... args)
{
    atomic_buffer buffer(dest);
    using format = make_format< String, Args... >;
    /* encode with format 0 */
    //encode< Args... >(buffer, format::data(), args...);
    /* encode with format 1 */
    encode< format, Args... >(buffer, args...);
    /* encode without format */
    //encode< Args... >(buffer, args...);
    std::cout << "format: \"" << format::data() << "\"\n";
    std::cout << "encoded-size: \"" << buffer.capacity() << "\"\n";
    return buffer.capacity();
}

#define log(buffer, fmt, ...) log_impl< typestring_is(fmt) >(buffer, ##__VA_ARGS__)

int main(int argc, char* argv[])
{
    char buffer[512];

    log(buffer, "hello {} {}", 123, "world");
    log(buffer, "test!");
    log(buffer, "yahoo! {}", 9999ul);
    log(buffer, "yahoo! {}", 9999l);

    //std::cout << std::boolalpha << type_format_is< int8_t >("i8") << '\n';
    //std::cout << std::boolalpha << type_format_is< int8_t >("i9") << '\n';

    return 0;
}
