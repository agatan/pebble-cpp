#ifndef __PEBBLE_SYNTAX_AST_FWD_HPP_
#define __PEBBLE_SYNTAX_AST_FWD_HPP_

#include <boost/variant.hpp>
#include <memory>

namespace pebble {
  namespace syntax {
    namespace ast {

#define PEBBLE_MAKE_NODE(name) class name; using name##_ptr = std::shared_ptr<name>

      PEBBLE_MAKE_NODE(int_const);
      PEBBLE_MAKE_NODE(ident);
      PEBBLE_MAKE_NODE(bool_const);
      PEBBLE_MAKE_NODE(unit_const);
      PEBBLE_MAKE_NODE(apply);
      PEBBLE_MAKE_NODE(bool_negative);
      PEBBLE_MAKE_NODE(negative);

      using expression = boost::variant <
        int_const_ptr,
        ident_ptr,
        bool_const_ptr,
        unit_const_ptr,
        apply_ptr,
        bool_negative_ptr,
        negative_ptr
          >;

    } // namespace ast
  } // namespace syntax
} // namespace pebble

#endif
