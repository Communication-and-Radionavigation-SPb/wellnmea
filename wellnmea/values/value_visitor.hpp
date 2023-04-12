#pragma once

#include <wellnmea/types_def.hpp>

namespace wellnmea
{
  namespace values
  {
    template <typename _Visitable>
    struct visitor
    {
      virtual void visit(const _Visitable *) = 0;
    };

    struct visitor_base
    {
      virtual ~visitor_base() {}
    };
  } // namespace values

} // namespace wellnmea
