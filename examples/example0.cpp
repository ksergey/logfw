/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include "logfw/make_format.hpp"
#include "logfw/common.hpp"
#include "logfw/encoder.hpp"
#include "logfw/decoder.hpp"
#include "logfw/write.hpp"

using namespace logfw;

template< class String, std::size_t N, class... Args >
inline std::size_t log_impl(char (&buffer)[N], const Args&... args)
{
    using format = make_format< String, Args... >;
    /* encode with format */
    std::size_t bytes = encoder::encode< Args... >(buffer, args...);
    /* encode without format */
    //encode< Args... >(buffer, args...);

    static constexpr size_t max_buffer_size = encoder::max_bytes_required< Args... >()
        + format::size() + 1;
    static_assert( max_buffer_size < 2048, "" );

    std::cout << "--------------------\n";
    std::cout << "format: \"" << format::data() << "\"\n";
    std::cout << "encoded-size: \"" << bytes << "\"\n";
    std::cout << "max-buffer-size: \"" << (encoder::max_bytes_required< Args... >()) << "\"\n";
    std::cout << "max-buffer-size (with fmt): \"" << max_buffer_size << "\"\n";

    std::cout << "result: \"";
    write(std::cout, format::str(), buffer, bytes);
    std::cout << "\"\n";

    return bytes;
}

#define log(buffer, fmt, ...)                                                           \
    {                                                                                   \
        struct format_holder { static constexpr const char* data() { return fmt; } };   \
        log_impl< format_holder >(buffer, ##__VA_ARGS__);                               \
    }

int main(__unused int argc, __unused char* argv[])
{
    char buffer[512];
    const char* str1 = "Paratruper";
    string_view str2 = "Puper";
    std::string str3 = "Duper";
    int x = 5;

    //log(buffer, "hello {} {}", 123, "world");
    //log(buffer, "test!");
    //log(buffer, "test! {}", 123);
    //log(buffer, "yahoo! {}", 9999ul);
    //log(buffer, "yahoo! {} {}", 9999l, test);
    //log(buffer, "twime-session: EstablishmentAck [ keepaliveInterval={}, nextSeqNo={} ]", 9999l, test);
    //log(buffer, "XXXXX {+010.7} {}", 10.03, &x);
    std::cout << &x << '\n';
    log(buffer, "YYYY {} {} {} {} &x={}, {f.10}", str1, str2, str3, "HAHAHA", &x, 0.00000001);

    return 0;
}
