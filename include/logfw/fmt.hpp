/*
 * Copyright (c) 2016 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_fmt_021216180149_MADLIFE
#define MADLIFE_fmt_021216180149_MADLIFE

#include "format_visitor.hpp"

namespace logfw {

__force_inline std::ostream& operator<<(std::ostream& os, format_visitor& fv)
{
    fv.apply(os);
    return os;
}

} /* namespace logfw */

#endif /* MADLIFE_fmt_021216180149_MADLIFE */
