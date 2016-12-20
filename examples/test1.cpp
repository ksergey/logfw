/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <array>
#include <utility>

template< std::size_t N1, std::size_t... I1, std::size_t N2, std::size_t... I2 >
constexpr std::array< char const, N1 + N2 - 1 >
concat(char const (&a1)[N1], char const (&a2)[N2], std::index_sequence< I1... >, std::index_sequence< I2... >)
{
    return {{ a1[I1]..., a2[I2]... }};
}

template< std::size_t N1, std::size_t N2>
constexpr std::array< char const, N1 + N2 - 1 >
concat(char const (&a1)[N1], char const (&a2)[N2])
{
    return concat(a1, a2, std::make_index_sequence< N1 - 1 >{}, std::make_index_sequence< N2 >{});
}

constexpr const auto cc = concat("=====================hello", " world################3");

int main()
{
    std::cout << cc.data() << '\n';
    std::cout << "testggggggggggggggggggggggggggggggggg\n";
    return 0;
}
