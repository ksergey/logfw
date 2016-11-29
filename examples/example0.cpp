/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include "logfw/atomic_buffer.hpp"
#include "logfw/make_format.hpp"
#include "logfw/arg_io.hpp"
#include "logfw/type_string.hpp"
#include "logfw/encoder.hpp"
#include "logfw/print.hpp"

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
    std::cout << "max-buffer-size: \"" << (max_bytes_required< Args... >()) << "\"\n";
    std::cout << "max-buffer-size (with fmt): \"" << (max_bytes_required< Args... >() + format::size() + 1) << "\"\n";
    return buffer.capacity();
}

#define log(buffer, fmt, ...) log_impl< typestring_is(fmt) >(buffer, ##__VA_ARGS__)

int main(int argc, char* argv[])
{
    char buffer[512];
    const char* test = "Paratruper";

    log(buffer, "hello {} {}", 123, "world");
    log(buffer, "test!");
    log(buffer, "yahoo! {}", 9999ul);
    log(buffer, "yahoo! {}", 9999l);

    std::cout << "formatting result ----------------\n";
    auto used_bytes = log(buffer, "yahoo! \"{50}\" \"{-+10}\"", test, 5);
    print(std::cout, atomic_buffer{buffer, used_bytes});

    return 0;
}
