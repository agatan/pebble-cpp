#ifndef __PEBBLE_SYNTAX_AST_HPP_
#define __PEBBLE_SYNTAX_AST_HPP_

#include <pebble/syntax/ast_fwd.hpp>
#include <pebble/utils/stringify.hpp>

#include <vector>

namespace pebble {
  namespace syntax {

    namespace ast {

      class int_const_expr
      {
      private:
        int data_;

      public:
        int_const_expr() = default;
        explicit int_const_expr(int d): data_(d) {}

        std::string to_string() const { return std::to_string(data_); }
        int data() const { return data_; }
      };

      class ident_expr
      {
      private:
        std::string name_;

      public:
        ident_expr() = default;
        explicit ident_expr(std::string const& name): name_(name) {}

        std::string to_string() const { return "(IDENT " + name_ + ")"; }
        std::string const& name() const { return name_; }
      };

      class bool_const_expr
      {
      private:
        bool data_;

      public:
        bool_const_expr() = default;
        explicit bool_const_expr(bool d): data_(d) {}

        std::string to_string() const
        {
          if (data_)
            return "TRUE";
          else
            return "FALSE";
        }
        bool data() const { return data_; }
      };

      class unit_expr
      {
      public:
        std::string to_string() const { return "UNIT"; }
      };

      class apply_expr
      {
      private:
        expression function_;
        std::vector<expression> args_;

      public:
        apply_expr() = default;
        explicit apply_expr(expression const& f, std::vector<expression> const& a)
          : function_(f), args_(a) {}

        std::string to_string() const;
      };

      class bool_negative_expr
      {
      private:
        expression operand_;
      public:
        bool_negative_expr() = default;
        explicit bool_negative_expr(expression const& operand): operand_(operand) {}

        std::string to_string() const
        {
          return "(NOT " + utils::stringify(operand_) + ")";
        }
      };

      class negative_expr
      {
      private:
        expression operand_;
      public:
        negative_expr() = default;
        explicit negative_expr(expression const& operand): operand_(operand) {}

        std::string to_string() const
        {
          return "(NEG " + utils::stringify(operand_) + ")";
        }
      };

    } // namespace ast

  } // namespace syntax
} // namespace pebble

#endif
