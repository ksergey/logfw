/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <array>
#include <utility>
#include "static_string.hpp"

template< std::size_t N >
using const_string = std::array< const char, N >;

/* ----------------------- */

template< std::size_t N, std::size_t... I >
constexpr const_string< N > make_const_string(char const (&a)[N], std::index_sequence< I... >)
{
    return {{a[I]...}};
}

template< std::size_t N, std::size_t... I >
constexpr const_string< N + 1 > make_const_string(const char* const a, std::index_sequence< I... >)
{
    return {{a[I]..., '\0'}};
}

template< std::size_t N >
constexpr auto make_const_string(char const (&a)[N])
{
    return make_const_string(a, std::make_index_sequence< N >{});
}

template< std::size_t N >
constexpr auto make_const_string(const char* const a)
{
    return make_const_string< N >(a, std::make_index_sequence< N >{});
}

template< std::size_t N >
constexpr const_string< N > make_const_string(const const_string< N >& a)
{
    return a;
}

constexpr const_string< 2 > make_const_string(char ch)
{
    return {{ch, '\0'}};
}

/* ----------------------- */

template< std::size_t N1, std::size_t... I1, std::size_t N2, std::size_t... I2 >
constexpr const_string< N1 + N2 - 1 >
concat(const const_string< N1 >& a1, const const_string< N2 >& a2,
        std::index_sequence< I1... >, std::index_sequence< I2... >)
{
    return {{a1[I1]..., a2[I2]...}};
}

template< std::size_t N1, std::size_t N2 >
constexpr const_string< N1 + N2 - 1 >
concat(const const_string< N1 >& a1, const const_string< N2 >& a2)
{
    return concat(a1, a2, std::make_index_sequence< N1 - 1 >{}, std::make_index_sequence< N2 >{});
}

template< class Arg0, class Arg1 >
constexpr auto concat(const Arg0& a0, const Arg1& a1)
{
    return concat(make_const_string(a0), make_const_string(a1));
}

constexpr const_string< 1 > concat()
{
    return {{'\0'}};
}

template< class Arg0, class... Args >
constexpr auto concat(const Arg0& a0, const Args&... args)
{
    return concat(a0, concat(args...));
}

/* ----------------------- */

template<
    std::size_t Pos, std::size_t Len, std::size_t N,
    class = typename std::enable_if< Pos + Len < N >::type
>
constexpr const_string< Len + 1 > slice(const const_string< N >& s)
{
    return make_const_string< Len >(&s[Pos], std::make_index_sequence< Len >{});
}

template< std::size_t Pos, std::size_t Len, class Arg >
constexpr auto slice(const Arg& arg)
{
    return slice< Pos, Len >(make_const_string(arg));
}

/* ----------------------- */

using namespace ss;

constexpr const char* string_value = "hello world string";
constexpr const auto cc0 = concat("=====================hello", " world################3");
constexpr const auto cc1 = concat("xxxxxx ", 'y');
constexpr const auto cc2 = concat(cc0, cc1);
constexpr const auto cc3 = concat("hello", ' ', "world", '!');
constexpr const auto cc4 = concat("hello", ' ', "world", ' ', cc1);
constexpr const auto cc5 = slice< 0, 4 >("workaround");
constexpr const auto cc6 = make_const_string< 3 >(string_value);
constexpr const auto cc7 = slice< 0, 5 >(cc3);

template< std::size_t N >
void print(const const_string< N >& s)
{
    std::cout << s.size() << ": " << s.data() << '\n';
}

template< std::size_t N >
void print(const static_string< N >& s)
{
    std::cout << s.size() << ": " << s << '\n';
}

constexpr const auto x0 = make_static_string("the_very_long_string");
constexpr const auto x1 = x0.append(make_static_string("_shit"));
constexpr const auto x2 = x0.append("_shitty");
constexpr const auto x3 = make_static_string("-----").push_back('x').push_front('y');
constexpr const auto x4 = x0.slice< 0, 3 >();

int main()
{
    print(cc0);
    print(cc1);
    print(cc2);
    print(cc3);
    print(cc4);
    print(cc5);
    print(cc6);
    print(cc7);

    print(x0);
    print(x1);
    print(x2);
    print(x3);
    print(x3.pop_back().pop_front());
    print(x4);
    print(x4.pop_front().pop_back());
    print(x0.tail< 10 >());
    print(x0.head< 10 >());
    print(x4.tail< 3 >());
    print(x4.head< 3 >());

    return 0;
}
