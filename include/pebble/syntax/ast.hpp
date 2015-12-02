#ifndef __PEBBLE_SYNTAX_AST_HPP_
#define __PEBBLE_SYNTAX_AST_HPP_

#include <pebble/syntax/ast_fwd.hpp>

#include <vector>

namespace pebble {
  namespace syntax {

    namespace ast {

      template<class T>
      std::string stringify(T const& x)
      {
        return boost::apply_visitor([](auto& e){ return e->to_string(); }, x);
      }

      class int_const
      {
      private:
        int data_;

      public:
        int_const() : int_const(0) {};
        int_const(int d): data_(d) {}
        std::string to_string() const { return std::to_string(data_); }
        int data() const { return data_; }
      };

      class ident
      {
      private:
        std::string name_;

      public:
        ident(): ident("") {}
        ident(std::string const& name): name_(name) {}
        std::string to_string() const { return "(IDENT " + name_ + ")"; }
        std::string const& name() const { return name_; }
      };

      class bool_const
      {
      private:
        bool data_;

      public:
        bool_const(): bool_const(false) {}
        bool_const(bool d): data_(d) {}
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
        unit_const() {}
        std::string to_string() const { return "UNIT"; }
      };

      class apply
      {
      private:
        expression function_;
        std::vector<expression> args_;

      public:
        apply() : apply(std::make_shared<unit_const>(), {}) {}
        apply(expression const& f, std::vector<expression> const& a)
          : function_(f), args_(a) {}

        std::string to_string() const;
      };

    } // namespace ast

  } // namespace syntax
} // namespace pebble

#endif
