#ifndef __PEBBLE_SYNTAX_AST_HPP_
#define __PEBBLE_SYNTAX_AST_HPP_

#include <pebble/syntax/ast_fwd.hpp>
#include <pebble/utils/stringify.hpp>

#include <vector>

namespace pebble {
  namespace syntax {

    namespace ast {

      class int_const
      {
      private:
        int data_;

      public:
        int_const() = default;
        explicit int_const(int d): data_(d) {}

        std::string to_string() const { return std::to_string(data_); }
        int data() const { return data_; }
      };

      class ident
      {
      private:
        std::string name_;

      public:
        ident() = default;
        explicit ident(std::string const& name): name_(name) {}

        std::string to_string() const { return "(IDENT " + name_ + ")"; }
        std::string const& name() const { return name_; }
      };

      class bool_const
      {
      private:
        bool data_;

      public:
        bool_const() = default;
        explicit bool_const(bool d): data_(d) {}

        std::string to_string() const
        {
          if (data_)
            return "TRUE";
          else
            return "FALSE";
        }
        bool data() const { return data_; }
      };

      class unit_const
      {
      public:
        std::string to_string() const { return "UNIT"; }
      };

      class apply
      {
      private:
        expression function_;
        std::vector<expression> args_;

      public:
        apply() = default;
        explicit apply(expression const& f, std::vector<expression> const& a)
          : function_(f), args_(a) {}

        std::string to_string() const;
      };

      class bool_negative
      {
      private:
        expression operand_;
      public:
        bool_negative() = default;
        explicit bool_negative(expression const& operand): operand_(operand) {}

        std::string to_string() const
        {
          return "(NOT " + utils::stringify(operand_) + ")";
        }
      };

      class negative
      {
      private:
        expression operand_;
      public:
        negative() = default;
        explicit negative(expression const& operand): operand_(operand) {}

        std::string to_string() const
        {
          return "(NEG " + utils::stringify(operand_) + ")";
        }
      };

    } // namespace ast

  } // namespace syntax
} // namespace pebble

#endif
