#ifndef __PEBBLE_SYNTAX_AST_FWD_HPP_
#define __PEBBLE_SYNTAX_AST_FWD_HPP_

#include <boost/variant/variant.hpp>
#include <memory>

namespace pebble {
  namespace syntax {
    namespace ast {

      class type
      {
      private:
        std::string name_;

      public:
        type() = default;
        explicit type(std::string const& n) : name_(n) {}

        std::string to_string() const { return "(TYPE " + name_ + ")"; }
      };

#define PEBBLE_MAKE_NODE(name) class name; using name##_ptr = std::shared_ptr<name>

      PEBBLE_MAKE_NODE(int_const_expr);
      PEBBLE_MAKE_NODE(float_const_expr);
      PEBBLE_MAKE_NODE(ident_expr);
      PEBBLE_MAKE_NODE(bool_const_expr);
      PEBBLE_MAKE_NODE(unit_expr);
      PEBBLE_MAKE_NODE(apply_expr);
      PEBBLE_MAKE_NODE(bool_negative_expr);
      PEBBLE_MAKE_NODE(negative_expr);
      PEBBLE_MAKE_NODE(if_expr);
      PEBBLE_MAKE_NODE(binop_expr);
      PEBBLE_MAKE_NODE(block_expr);

      using expression = boost::variant <
        int_const_expr_ptr,
        float_const_expr_ptr,
        ident_expr_ptr,
        bool_const_expr_ptr,
        unit_expr_ptr,
        apply_expr_ptr,
        bool_negative_expr_ptr,
        negative_expr_ptr,
        if_expr_ptr,
        binop_expr_ptr,
        block_expr_ptr
          >;

      PEBBLE_MAKE_NODE(function_def);
      PEBBLE_MAKE_NODE(let_def);

      using definition = boost::variant <
        function_def_ptr,
        let_def_ptr
          >;

      PEBBLE_MAKE_NODE(def_stmt);
      PEBBLE_MAKE_NODE(expr_stmt);

      using statement = boost::variant <
        def_stmt_ptr,
        expr_stmt_ptr
          >;

    } // namespace ast
  } // namespace syntax
} // namespace pebble

#endif
