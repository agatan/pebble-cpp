#ifndef __PEBBLE_SYNTAX_GRAMMAR_HPP_
#define __PEBBLE_SYNTAX_GRAMMAR_HPP_

#include <pebble/syntax/ast.hpp>

#include <boost/spirit/home/x3.hpp>

namespace pebble {
  namespace syntax {
    namespace grammar {

      namespace x3 = boost::spirit::x3;

      using x3::lit;
      using x3::_attr;
      using x3::_val;
      using x3::lit;
      using x3::char_;

      x3::rule<class expression, ast::expression> const expression;

      template <typename T>
      auto keyword(T&& literal)
      {
        return x3::lexeme[
            lit(std::forward<T>(literal))
            >> !(
                char_('_')
                )
          ];
      }

      auto expression_def = lit("true")[
        ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::bool_const_expr>(true); })
      ];

      BOOST_SPIRIT_DEFINE(expression);

    } // namespace grammar
    using grammar::expression;

  } // namespace syntax
} // namespace pebble

#endif
