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

    } // namespace ast
  } // namespace syntax
} // namespace pebble
