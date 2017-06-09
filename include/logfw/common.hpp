/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_common_041216122507_MADLIFE
#define MADLIFE_common_041216122507_MADLIFE

#include <experimental/string_view>

#ifndef __force_inline
#   define __force_inline inline __attribute__((always_inline))
#endif

#ifndef __likely
#   define __likely(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef __unlikely
#   define __unlikely(x) __builtin_expect(static_cast< bool >(x), false)
#endif

namespace logfw {

/* c++17 string view */
using std::experimental::string_view;

} /* namespace logfw */

#endif /* MADLIFE_common_041216122507_MADLIFE */
