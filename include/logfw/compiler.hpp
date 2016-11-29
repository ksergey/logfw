/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_compiler_171116173517_MADLIFE
#define MADLIFE_compiler_171116173517_MADLIFE

#ifndef __force_inline
#   define __force_inline inline __attribute__((always_inline))
#endif

#ifndef __likely
#   define __likely(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef __unlikely
#   define __unlikely(x) __builtin_expect(static_cast< bool >(x), false)
#endif

#endif /* MADLIFE_compiler_171116173517_MADLIFE */
