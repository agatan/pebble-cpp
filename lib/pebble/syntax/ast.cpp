#include <pebble/syntax/ast.hpp>
#include <pebble/utils/stringify.hpp>

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

      std::string block_expr::to_string() const
      {
        std::string result("(BLOCK");
        for (auto it = stmts_.begin(); it != stmts_.end(); ++it) {
          result += " " + utils::stringify(*it);
        }
        result += " " + utils::stringify(ret_);
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
