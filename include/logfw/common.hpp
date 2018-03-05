/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_common_041216122507_MADLIFE
#define MADLIFE_common_041216122507_MADLIFE

// Probe `string_view` implementations
#ifdef __has_include
#   if (__has_include(<string_view>) && __cplusplus > 201402L)
#       include <string_view>
namespace logfw {
    using std::basic_string_view;
    namespace string_view_literals = std::string_view_literals;
} /* namespace logfw */
#   elif (__has_include(<experimental/string_view>) && __cplusplus >= 201402L)
#       include <experimental/string_view>
namespace logfw {
    using std::experimental::basic_string_view;
    namespace string_view_literals = std::experimental::string_view_literals;
} /* namespace logfw */
#   else
#       error "Missing <string_view>"
#   endif
#endif /* __has_include */

#ifndef LOGFW_FORCE_INLINE
#   define LOGFW_FORCE_INLINE inline __attribute__((always_inline))
#endif

#ifndef LOGFW_LIKELY
#   define LOGFW_LIKELY(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef LOGFW_UNLIKELY
#   define LOGFW_UNLIKELY(x) __builtin_expect(static_cast< bool >(x), false)
#endif

namespace logfw {

// c++17 string view
using string_view = basic_string_view< char >;
using wstring_view = basic_string_view< wchar_t >;

} /* namespace logfw */

#endif /* MADLIFE_common_041216122507_MADLIFE */
