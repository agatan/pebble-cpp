#ifndef __PEBBLE_SYNTAX_EXPRESSION_HPP_
#define __PEBBLE_SYNTAX_EXPRESSION_HPP_

#include <pebble/syntax/ast_fwd.hpp>
#include <pebble/utils/stringify.hpp>

namespace pebble {
  namespace syntax {
    namespace ast {
      // expressions
      template <class T, typename... Args>
      expression make_expr(Args&&... args)
      {
        return std::move(std::make_shared<T>(std::forward<Args>(args)...));
      }

      class int_const_expr: public ast_node_base
      {
      private:
        int data_;

      public:
        int_const_expr() = default;
        explicit int_const_expr(int d): data_(d) {}

        std::string to_string() const { return std::to_string(data_); }
        int data() const { return data_; }
      };

      class float_const_expr: public ast_node_base
      {
      private:
        double data_;

      public:
        float_const_expr() = default;
        explicit float_const_expr(double d): data_(d) {}

        std::string to_string() const { return std::to_string(data_); }
        double data() const { return data_; }
      };

      class ident_expr: public ast_node_base
      {
      private:
        std::string name_;

      public:
        ident_expr() = default;
        explicit ident_expr(std::string const& name): name_(name) {}

        std::string to_string() const { return "(IDENT " + name_ + ")"; }
        std::string const& name() const { return name_; }
      };

      class bool_const_expr: public ast_node_base
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

      class unit_expr: public ast_node_base
      {
      public:
        std::string to_string() const { return "UNIT"; }
      };

      class apply_expr: public ast_node_base
      {
      private:
        expression function_;
        std::vector<expression> args_;

      public:
        apply_expr() = default;
        explicit apply_expr(expression const& f, std::vector<expression> const& a)
          : function_(f), args_(a) {}

        std::string to_string() const;

        expression const& function() const { return function_; }
        std::vector<expression> const& args() const { return args_; }
      };

      class bool_negative_expr: public ast_node_base
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

      class negative_expr: public ast_node_base
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

      class if_expr: public ast_node_base
      {
      private:
        expression cond_;
        expression then_;
        expression else_;
      public:
        if_expr() = default;
        explicit if_expr(expression const& c, expression const& t, expression const& e)
          : cond_(c), then_(t), else_(e) {}
        explicit if_expr(expression const& c, expression const& t)
          : cond_(c), then_(t), else_(std::make_shared<unit_expr>()) {}

        std::string to_string() const
        {
          return "(IF " + utils::stringify(cond_) + " " +
            utils::stringify(then_) + " " + utils::stringify(else_) + ")";
        }
      };

      class binop_expr: public ast_node_base
      {
      private:
        std::string op_;
        expression lhs_;
        expression rhs_;
      public:
        binop_expr() = default;
        explicit binop_expr(
            std::string const& op,
            expression const& l,
            expression const& r)
          : op_(op), lhs_(l), rhs_(r)
        {}

        std::string to_string() const
        {
          return "(" + op_ + " " + utils::stringify(lhs_) +
            " " + utils::stringify(rhs_) + ")";
        }

        expression const& lhs() const { return lhs_; }
        expression const& rhs() const { return rhs_; }
        std::string const& op() const { return op_; }
      };

      class block_expr: public ast_node_base
      {
      private:
        std::vector<statement> stmts_;
        bool is_expr;
      public:
        block_expr() = default;
        explicit block_expr(std::vector<statement> const& e);
        block_expr(std::vector<statement> const& s, expression const& e);

        std::string to_string() const;
      };

    } // namespace ast
  } // namespace syntax
} // namespace pebble

#endif
