#ifndef __PEBBLE_UTILS_STRINGIFY_HPP_
#define __PEBBLE_UTILS_STRINGIFY_HPP_

#include <boost/variant.hpp>
#include <string>

namespace pebble {
  namespace utils {
    template <class T>
    std::string stringify(T const& node)
    {
      return boost::apply_visitor([](auto& x) { return x->to_string(); }, node);
    }
  } // namespace utils
} // namespace pebble

#endif
