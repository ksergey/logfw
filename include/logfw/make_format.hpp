/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_make_format_291116173253_MADLIFE
#define MADLIFE_make_format_291116173253_MADLIFE

#include "common.hpp"
#include "detail/meta.hpp"
#include "detail/type_format.hpp"

namespace logfw {
namespace detail {

/* represent a type in char-sequence */
template< class T, typename FormatSpec >
struct format_type
{
    using raw_type = clear_type< T >;
    /* true if no formating flags was in {} */
    static constexpr const bool empty_spec = std::is_same< FormatSpec, null_type >::value;

    /* fmt0="{type" */
    using fmt0 = append< char_list< '{' >, typename type_format< raw_type >::type >;
    /* fmt1="{type:" or fmt1="{type" */
    using fmt1 = typename std::conditional< empty_spec, fmt0, append< fmt0, ch< ':' > > >::type;
    /* fmt2="{type:spec" or fmt2="{type" */
    using fmt2 = append< fmt1, FormatSpec >;

    /* type="{type:spec}" or type="{type}" */
    using type = append< fmt2, ch< '}' > >;
};

/* static_assert helper */
template< class T >
struct always_false
    : std::false_type
{};

/* find '}' in charlist sequence */
template< class InputList, class OutputList, std::size_t N >
struct find_close_brace;
template< class InputList, class OutputList >
struct find_close_brace< list< ch< '}' >, InputList >, OutputList, 1 >
{
    using before = OutputList;
    using after = InputList;
};
template< char C, class InputList, class OutputList, std::size_t N >
struct find_close_brace< list< ch< C >, InputList >, OutputList, N >
    : find_close_brace< InputList, append< OutputList, ch< C > >, N >
{
    static_assert( C != '{', "" );
};
template< class OutputList, std::size_t N >
struct find_close_brace< null_type, OutputList, N >
{
    /* make diagnostic easier */
    static_assert( always_false< OutputList >::value, "close brace not found" );
};

/* format string */
template< class StringList, class TypeList >
struct format_impl;
template<>
struct format_impl< null_type, null_type >
{
    using type = null_type;
};
template< class TypeList >
struct format_impl< null_type, TypeList >
{
    using type = null_type;
    static_assert( detail::always_false< TypeList >::value,
            "There are more vars than format tokens" );
};
template< class StringList, class TypeList >
struct format_impl< list< ch< '{' >, list< ch< '{' >, StringList > >, TypeList >
{
    /* replace {{ -> { */
    using type = list< ch< '{' >, list < ch< '{' >, typename format_impl< StringList, TypeList >::type > >;
};
template< class StringList, class TypeList >
struct format_impl< list< ch< '}' >, list< ch< '}' >, StringList > >, TypeList >
{
    /* replace }} -> } */
    using type = list< ch< '}' >, list < ch< '}' >, typename format_impl< StringList, TypeList >::type > >;
};
template< class StringList, class TypeList >
struct format_impl< list< ch< '}' >, StringList >, TypeList >
{
    /* found close brace without open brace */
    static_assert( always_false< TypeList >::value,
            "Not expected close brace" );
    using type = null_type;
};
template< class StringList, class TypeList >
struct format_impl< list< ch< '{' >, StringList >, TypeList >
{
    /* found type specifier */
    using close_brace = find_close_brace< StringList, null_type, 1 >;
    /* format specifier without type, i.e. <flags><width>.<precision> */
    using format_spec = typename close_brace::before;
    /* after close brace string */
    using rest = typename close_brace::after;

    static_assert( !std::is_same< TypeList, null_type >::value,
            "not enought argument for formatting string" );

    /* get current argument type */
    using T = head_type< TypeList >;
    /* construct format specifier */
    using format_string = typename format_type< T, format_spec >::type;
    using format_rest = typename format_impl< rest, tail_type< TypeList > >::type;
    using type = append< format_string, format_rest >;
};
template< class C, class StringList, class TypeList >
struct format_impl< list< C, StringList >, TypeList >
{
    /* iterate to next symbol */
    using type = list< C, typename format_impl< StringList, TypeList >::type >;
};

} /* namespace detail */

/**
 * construct format object.
 *
 * make_format<...>::data() - const char* line
 * make_format<...>::size() - size of line
 */
template< class StringHolder, class... Args >
using make_format = detail::stringify<
    typename detail::format_impl<
        detail::string_list< StringHolder >,
        detail::make_list< Args...>
    >::type
>;

} /* namespace logfw */

#endif /* MADLIFE_make_format_291116173253_MADLIFE */
