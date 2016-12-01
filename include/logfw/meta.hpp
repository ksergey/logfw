/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_meta_291116165708_MADLIFE
#define MADLIFE_meta_291116165708_MADLIFE

#include <type_traits>
#include "string_view.hpp"
#include "compiler.hpp"

namespace logfw {

/* null list type */
struct null_type
{
    using head = null_type;
    using tail = null_type;
};

/* typelist primitive */
template< class Head, class Tail >
struct list
{
    using head = Head;
    using tail = Tail;
};

/* head type from list */
template< class List >
using head_type = typename List::head;
/* tail type from list */
template< class List >
using tail_type = typename List::tail;

/* make list implementation  */
template< class... Args >
struct make_list_impl;
template< class Head, class... Rest >
struct make_list_impl< Head, Rest... >
{ using type = list< Head, typename make_list_impl< Rest... >::type >; };
template<>
struct make_list_impl<>
{ using type = null_type; };

/* make typelist from input Args */
template< class... Args >
using make_list = typename make_list_impl< Args... >::type;

/* append type to typelist implementation */
template< class List, class T >
struct append_impl;
template<>
struct append_impl< null_type, null_type >
{ using type = null_type; };
template< class T >
struct append_impl< null_type, T >
{ using type = make_list< T >; };
template< class Head, class T >
struct append_impl< null_type, list< Head, T > >
{ using type = list< Head, T >; };
template< class Head, class Tail, class T >
struct append_impl< list< Head, Tail >, T >
{ using type = list< Head, typename append_impl< Tail, T >::type >; };

/* append type to typelist */
template< class List, class T >
using append = typename append_impl< List, T >::type;

/* character class */
template< char Value >
struct ch
    : std::integral_constant< char, Value >
{};

/* character list implementation */
template< char C, char... Chars >
struct char_list_impl
{ using type = list< ch< C >, typename char_list_impl< Chars... >::type >; };
template< char C >
struct char_list_impl< C >
{ using type = list< ch< C >, null_type >; };

/* typelist of ch< Char > from input character list */
template< char... Chars >
using char_list = typename char_list_impl< Chars... >::type;

/* character accessor from char_list implementation */
template< class Str, std::size_t Pos, char C >
struct string_list_impl
{
    using next_piece = typename string_list_impl< Str, Pos + 1, Str::data()[Pos + 1] >::type;
    using type = list< ch< C >, next_piece >;
};
template< class Str, std::size_t Pos >
struct string_list_impl< Str, Pos, '\0' >
{ using type = null_type; };

/* character accessor from char_list */
template< class Str >
using string_list = typename string_list_impl< Str, 0, Str::data()[0] >::type;

/* convert from string type to const char* */
template< class TypeList, char... Chars >
struct stringify;
template< char C, class Rest, char... Chars >
struct stringify< list< ch< C >, Rest >, Chars... >
    : stringify< Rest, Chars..., C >
{};
template<>
struct stringify< null_type >
{
    static __force_inline const char* data() noexcept
    {
        return "";
    }

    static __force_inline constexpr std::size_t size() noexcept
    {
        return 0;
    }

    static __force_inline string_view str() noexcept
    {
        return string_view();
    }
};
template< char... Chars >
struct stringify< null_type, Chars... >
{
    static __force_inline const char* data() noexcept
    {
        static constexpr const char str[] = {Chars..., '\0'};
        return str;
    }

    static __force_inline constexpr std::size_t size() noexcept
    {
        return sizeof...(Chars);
    }

    static __force_inline string_view str() noexcept
    {
        return string_view(data(), size());
    }
};

/* some helpers */
template< class T >
using clear_type = typename std::conditional<
    std::is_pointer< T >::value,
    typename std::add_pointer<
            typename std::remove_cv<
                    typename std::remove_pointer< T >::type
                >::type
        >::type,
    typename std::remove_cv< T >::type
>::type;

} /* namespace logfw */

#endif /* MADLIFE_meta_291116165708_MADLIFE */
