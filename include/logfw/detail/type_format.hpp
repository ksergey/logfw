/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_type_format_291116174657_MADLIFE
#define MADLIFE_type_format_291116174657_MADLIFE

#include <cstdint>
#include <string>
#include "../common.hpp"
#include "meta.hpp"

namespace logfw {
namespace detail {

/* handle compile-time types */
template< class T >
struct type_format;

template<>
struct type_format< int8_t >
{ using type = char_list< 'i', '8' >; };
template<>
struct type_format< uint8_t >
{ using type = char_list< 'u', '8' >; };
template<>
struct type_format< int16_t >
{ using type = char_list< 'i', '1', '6' >; };
template<>
struct type_format< uint16_t >
{ using type = char_list< 'u', '1', '6' >; };
template<>
struct type_format< int32_t >
{ using type = char_list< 'i', '3', '2' >; };
template<>
struct type_format< uint32_t >
{ using type = char_list< 'u', '3', '2' >; };
template<>
struct type_format< int64_t >
{ using type = char_list< 'i', '6', '4' >; };
template<>
struct type_format< uint64_t >
{ using type = char_list< 'u', '6', '4' >; };
template<>
struct type_format< char* >
{ using type = char_list< 's' >; };
template<>
struct type_format< std::string >
{ using type = char_list< 's' >; };
template< std::size_t N >
struct type_format< char[N] >
{ using type = char_list< 's' >; };
template<>
struct type_format< string_view >
{ using type = char_list< 's' >; };
template<>
struct type_format< char >
{ using type = char_list< 'c' >; };
template<>
struct type_format< double >
{ using type = char_list< 'd' >; };
template<>
struct type_format< float >
{ using type = char_list< 'f' >; };

} /* namespace detail */
} /* namespace logfw */

#endif /* MADLIFE_type_format_291116174657_MADLIFE */
