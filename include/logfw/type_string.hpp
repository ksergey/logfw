/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_type_string_291116182401_MADLIFE
#define MADLIFE_type_string_291116182401_MADLIFE

#include "deps/typestring.hh"

namespace logfw {

template< char... Chars >
using type_string = irqus::typestring< Chars... >;

} /* namespace logfw */

#endif /* MADLIFE_type_string_291116182401_MADLIFE */
