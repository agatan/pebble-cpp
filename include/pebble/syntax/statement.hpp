#ifndef __PEBBLE_SYNTAX_STATEMENT_HPP_
#define __PEBBLE_SYNTAX_STATEMENT_HPP_

#include <pebble/syntax/ast_fwd.hpp>
#include <pebble/utils/stringify.hpp>

namespace pebble {
  namespace syntax {
    namespace ast {

      class def_stmt: public ast_node_base
      {
      private:
        definition def_;

      public:
        def_stmt() = default;
        explicit def_stmt(definition const& def): def_(def) {}

        std::string to_string() const
        {
          return "(STMT " + utils::stringify(def_) + ")";
        }
      };

      class expr_stmt: public ast_node_base
      {
      private:
        expression expr_;

      public:
        expr_stmt() = default;
        explicit expr_stmt(expression const& expr) : expr_(expr) {}

        std::string to_string() const
        {
          return "(STMT " + utils::stringify(expr_) + ")";
        }

        expression const& expr() const { return expr_; }
      };

      template <typename T, typename... Args>
      statement make_stmt(Args&&... args)
      {
        return std::move(std::make_shared<T>(std::forward<Args>(args)...));
      }

    } // namespace ast
  } // namespace syntax
} // namespace pebble

#endif
