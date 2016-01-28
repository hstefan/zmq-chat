#pragma once

#include <iterator>

namespace hst {

template <class InputIt, class OutputIt, class Pred>
OutputIt copy_while(InputIt first, InputIt last, OutputIt dest, Pred pred) {
  while (first != last && pred(*first))
    *dest++ = *first++;

  return dest;
}
} // namespace hst
