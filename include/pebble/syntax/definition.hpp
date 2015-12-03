#ifndef __PEBBLE_SYNTAX_DEFINITION_HPP_
#define __PEBBLE_SYNTAX_DEFINITION_HPP_

#include <pebble/syntax/ast_fwd.hpp>

#include <boost/optional/optional.hpp>

namespace pebble {
  namespace syntax {
    namespace ast {
      // definitions

      class function_def
      {
      private:
        std::string name_;
        std::vector<std::pair<std::string, type>> args_;
        type ret_;
        expression body_;

      public:
        function_def() = default;
        explicit function_def(
            std::string const& name,
            std::vector<std::pair<std::string, type>> const& args,
            type const& ret,
            expression const& body)
          : name_(name), args_(args), ret_(ret), body_(body)
        {}

        std::string to_string() const;
      };

      class let_def
      {
      private:
        std::string name_;
        boost::optional<ast::type> type_;
        expression init_;

      public:
        let_def() = default;
        explicit let_def(std::string const& name, expression const& init)
          : name_(name), type_(boost::none), init_(init)
        {}

        explicit let_def(
            std::string const& name,
            ast::type const& type,
            expression const& init)
          : name_(name), type_(type), init_(init)
        {}

        std::string to_string() const;
      };

      template <class T, typename... Args>
      definition make_definition(Args&&... args)
      {
        return std::move(std::make_shared<T>(std::forward<Args>(args)...));
      }
    } // namespace ast
  } // namespace syntax
} // namespace pebble

#endif
