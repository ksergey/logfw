/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include "logfw/atomic_buffer.hpp"
#include "logfw/make_format.hpp"
#include "logfw/arg_io.hpp"
#include "logfw/type_string.hpp"

using namespace logfw;

template< class String, class... Args >
inline void log_impl(atomic_buffer& buffer, const Args&... args)
{
    using format = make_format< String, Args... >;
    std::cout << "format: \"" << format::data() << "\"\n";
}

#define log(buffer, fmt, ...) log_impl< typestring_is(fmt) >(buffer, ##__VA_ARGS__)

int main(int argc, char* argv[])
{
    char mem[512];
    atomic_buffer buffer(mem);

    log(buffer, "hello {} {}", 123, "world");
    log(buffer, "test!");
    log(buffer, "yahoo! {}", 9999ul);
    log(buffer, "yahoo! {}", 9999l);

    //std::cout << std::boolalpha << type_format_is< int8_t >("i8") << '\n';
    //std::cout << std::boolalpha << type_format_is< int8_t >("i9") << '\n';

    return 0;
}
