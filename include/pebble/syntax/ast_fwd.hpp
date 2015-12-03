#ifndef __PEBBLE_SYNTAX_AST_FWD_HPP_
#define __PEBBLE_SYNTAX_AST_FWD_HPP_

#include <boost/variant.hpp>
#include <memory>

namespace pebble {
  namespace syntax {
    namespace ast {

#define PEBBLE_MAKE_NODE(name) class name; using name##_ptr = std::shared_ptr<name>

      PEBBLE_MAKE_NODE(int_const_expr);
      PEBBLE_MAKE_NODE(ident_expr);
      PEBBLE_MAKE_NODE(bool_const_expr);
      PEBBLE_MAKE_NODE(unit_expr);
      PEBBLE_MAKE_NODE(apply_expr);
      PEBBLE_MAKE_NODE(bool_negative_expr);
      PEBBLE_MAKE_NODE(negative_expr);
      PEBBLE_MAKE_NODE(if_expr);
      PEBBLE_MAKE_NODE(binop_expr);

      using expression = boost::variant <
        int_const_expr_ptr,
        ident_expr_ptr,
        bool_const_expr_ptr,
        unit_expr_ptr,
        apply_expr_ptr,
        bool_negative_expr_ptr,
        negative_expr_ptr,
        if_expr_ptr,
        binop_expr_ptr
          >;

    } // namespace ast
  } // namespace syntax
} // namespace pebble

#endif
