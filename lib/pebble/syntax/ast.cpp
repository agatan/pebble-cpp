#include <pebble/syntax/ast.hpp>
#include <pebble/utils/stringify.hpp>

#include <boost/variant/get.hpp>

#include <algorithm>

namespace pebble {
  namespace syntax {
    namespace ast {

      std::string apply_expr::to_string() const
      {
        std::string function{ utils::stringify(function_) };

        std::string result("(APPLY " + function);

        for (auto it = args_.begin(); it != args_.end(); ++it) {
          result += " " + utils::stringify(*it);
        }

        result += ")";

        return result;
      }

      block_expr::block_expr(std::vector<statement> const& s)
        : stmts_(s)
      {
        auto const& last(s.back());
        if (!boost::get<ast::expr_stmt_ptr>(&last)) {
          is_expr = false;
          return;
        }
        ast::expr_stmt_ptr const& last_e(boost::get<ast::expr_stmt_ptr>(last));
        if (boost::get<ast::block_expr_ptr>(&last_e->expr()) ||
            boost::get<ast::if_expr_ptr>(&last_e->expr()))
        {
          is_expr = true;
        } else {
          is_expr = false;
        }
      }

      block_expr::block_expr(std::vector<statement> const& s, expression const& e)
        : is_expr(true)
      {
        stmts_ = s;
        stmts_.push_back(make_stmt<expr_stmt>(e));
      }

      std::string block_expr::to_string() const
      {
        std::string result("(BLOCK");
        if (is_expr) {
          result += "_EXPR";
        }
        for (auto it = stmts_.begin(); it != stmts_.end(); ++it) {
          result += " " + utils::stringify(*it);
        }
        result += ")";
        return result;
      }

      std::string function_def::to_string() const
      {
        auto result("(DEF " + name_ + " (");

        for (auto it = args_.begin(); it != args_.end(); ++it) {
          result += "(" + it->first + " " + it->second.to_string() + ")";
          if (it + 1 != args_.end())
            result += " ";
        }

        result += ")";

        result += " (RET " + ret_.to_string() + ") (BODY " +
          utils::stringify(body_) + "))";

        return result;
      }

      std::string let_def::to_string() const
      {
        std::string result("(LET ");
        if (type_) {
          result += "(" + name_ + " " + type_->to_string() + ")";
        } else {
          result += name_;
        }
        result += " " + utils::stringify(init_) + ")";
        return result;
      }

    } // namespace ast
  } // namespace syntax
} // namespace pebble
