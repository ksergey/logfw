/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <array>
#include <utility>

template< std::size_t N >
using const_string = std::array< const char, N >;

template< std::size_t N, std::size_t... I >
constexpr const_string< N > to_const_string(char const (&a)[N], std::index_sequence< I... >)
{
    return {{ a[I]... }};
}

template< std::size_t N, std::size_t... I >
constexpr const_string< N > to_const_string(char const* a, std::index_sequence< I... >)
{
    return {{ a[I]... }};
}

template< std::size_t N >
constexpr const_string< N > to_const_string(char const (&a)[N])
{
    return to_const_string(a, std::make_index_sequence< N >{});
}

template< std::size_t N >
constexpr const_string< N > to_const_string(const const_string< N >& a)
{
    return a;
}

constexpr const_string< 2 > to_const_string(char ch)
{
    return {{ ch, '\0' }};
}

template< std::size_t N1, std::size_t... I1, std::size_t N2, std::size_t... I2 >
constexpr const_string< N1 + N2 - 1 >
concat(const const_string< N1 >& a1, const const_string< N2 >& a2,
        std::index_sequence< I1... >, std::index_sequence< I2... >)
{
    return {{ a1[I1]..., a2[I2]... }};
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
    return concat(to_const_string(a0), to_const_string(a1));
}

constexpr const_string< 1 > concat()
{
    return {{ '\0' }};
}

template< class Arg0, class... Args >
constexpr auto concat(const Arg0& a0, const Args&... args)
{
    return concat(a0, concat(args...));
}

constexpr const char* string_value = "string123";
constexpr const auto cc0 = concat("=====================hello", " world################3");
constexpr const auto cc1 = concat("xxxxxx ", 'y');
constexpr const auto cc2 = concat(cc0, cc1);
constexpr const auto cc3 = concat("hello", ' ', "world", '!');
constexpr const auto cc4 = concat("hello", ' ', "world", ' ', cc1);

int main()
{
    std::cout << cc0.data() << '\n';
    std::cout << cc1.data() << '\n';
    std::cout << cc2.data() << '\n';
    std::cout << cc3.data() << '\n';
    std::cout << cc4.data() << '\n';
    return 0;
}
