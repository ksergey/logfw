// ------------------------------------------------------------
// Copyright (c) 2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_compiler_290618132957
#define KSERGEY_compiler_290618132957

#ifndef LOGFW_FORCE_INLINE
#   define LOGFW_FORCE_INLINE inline __attribute__((always_inline))
#endif

#ifndef LOGFW_LIKELY
#   define LOGFW_LIKELY(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef LOGFW_UNLIKELY
#   define LOGFW_UNLIKELY(x) __builtin_expect(static_cast< bool >(x), false)
#endif

#endif /* KSERGEY_compiler_290618132957 */
