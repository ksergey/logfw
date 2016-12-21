/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_string_211216152515
#define KSERGEY_static_string_211216152515

#include <utility>
#include <type_traits>
#include <iostream>

namespace ss {

template< std::size_t N >
class static_string final
{
private:
    const char data_[N] = {};

public:
    using const_iterator = const char*;

    constexpr static_string()
    {}

    constexpr static_string(const char (&s)[N + 1])
        : static_string(s, std::make_index_sequence< N >{})
    {}

    template< std::size_t... I >
    constexpr static_string(const char* s, std::index_sequence< I... >)
        : data_{s[I]...}
    {}

    template<
        class... Chars,
        class = std::enable_if_t< sizeof...(Chars) == N >
    >
    constexpr static_string(Chars&&... chars)
        : data_{std::forward< Chars >(chars)...}
    {}

    constexpr const char* data() const
    {
        return data_;
    }

    constexpr std::size_t size() const
    {
        return N;
    }

    constexpr char operator[](std::size_t pos) const
    {
        return data_[pos];
    }

    constexpr const_iterator begin() const
    {
        return data_;
    }

    constexpr const_iterator end() const
    {
        return data_ + N;
    }

    constexpr const_iterator cbegin() const
    {
        return data_;
    }

    constexpr const_iterator cend() const
    {
        return data_ + N;
    }

    template< std::size_t M >
    constexpr static_string< N + M > append(const static_string< M >& s) const
    {
        return append(s, std::make_index_sequence< N >{}, std::make_index_sequence< M >{});
    }

    template< std::size_t M >
    constexpr static_string< N + M > append(const char (&s)[M]) const
    {
        return append(s, std::make_index_sequence< N >{}, std::make_index_sequence< M >{});
    }

    template<
        std::size_t Pos,
        std::size_t Len,
        class = std::enable_if_t< (Pos + Len <= N && Len > 0) >
    >
    constexpr static_string< Len > slice() const
    {
        return { data_ + Pos, std::make_index_sequence< Len >{} };
    }

    template<
        std::size_t Len = 1,
        class = std::enable_if_t< (Len <= N) >
    >
    constexpr static_string< Len > tail() const
    {
        return slice< N - Len, Len >();
    }

    template<
        std::size_t Len = 1,
        class = std::enable_if_t< (Len <= N) >
    >
    constexpr static_string< Len > head() const
    {
        return slice< 0, Len >();
    }

    constexpr static_string< N + 1 > push_back(char ch) const
    {
        return push_back(ch, std::make_index_sequence< N >{});
    }

    constexpr static_string< N + 1 > push_front(char ch) const
    {
        return push_front(ch, std::make_index_sequence< N >{});
    }

    template< class = std::enable_if_t< (N > 1) > >
    constexpr static_string< N - 1 > pop_back() const
    {
        return {data_, std::make_index_sequence< N - 1 >{}};
    }

    template< class = std::enable_if_t< (N > 1) > >
    constexpr static_string< N - 1 > pop_front() const
    {
        return {data_ + 1, std::make_index_sequence< N - 1 >{}};
    }

private:
    template< class Str, std::size_t... NI, std::size_t... MI >
    constexpr static_string< sizeof...(NI) + sizeof...(MI) >
    append(const Str& s, std::index_sequence< NI... >, std::index_sequence< MI... >) const
    {
        return {data_[NI]..., s[MI]...};
    }

    template< std::size_t... I >
    constexpr static_string< N + 1 > push_back(char ch, std::index_sequence< I... >) const
    {
        return {data_[I]..., ch};
    }

    template< std::size_t... I >
    constexpr static_string< N + 1 > push_front(char ch, std::index_sequence< I... >) const
    {
        return {ch, data_[I]...};
    }
};

template< std::size_t N >
inline constexpr static_string< N - 1 > make_static_string(const char (&s)[N])
{
    return static_string< N - 1 >(s);
}

template< std::size_t N >
inline std::ostream& operator<<(std::ostream& os, const static_string< N >& s)
{
    os.write(s.data(), s.size());
    return os;
}

} /* namespace ss */

#endif /* KSERGEY_static_string_211216152515 */
