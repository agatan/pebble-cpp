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


      namespace helper {
        template <typename T>
          auto keyword(T&& literal)
          {
            return x3::lexeme[
              lit(std::forward<T>(literal))
                >> !(
                    char_('a', 'z')
                    | char_('_')
                    | char_('A', 'Z')
                    | char_('0', '9')
                    )
            ];
          }

        struct pebble_strict_float_policies : public x3::strict_real_policies<double>
        {
          static bool const allow_leading_dot = false;
          static bool const allow_trailing_dot = false;
        };
        x3::real_parser<double, pebble_strict_float_policies> const fp_;

        struct assign_action
        {
          template <typename Context>
          void operator()(Context& ctx)
          {
            _val(ctx) = _attr(ctx);
          }
        };
      } // namespace helper


      x3::rule<class expression, ast::expression> const expression;
      x3::rule<class constant, ast::expression> const constant;
      x3::rule<class primary_expr, ast::expression> const primary_expr;
      x3::rule<class postfix_expr, ast::expression> const postfix_expr;
      x3::rule<class unary_expr, ast::expression> const unary_expr;

      x3::rule<class identifier, std::string> const identifier;
      x3::rule<class real_args, std::vector<ast::expression>> const real_args;


      auto sep = (lit('\n') | "\r\n");

      auto identifier_def = x3::lexeme[
          x3::lower >> *(x3::alnum | char_('_'))
        ]
        ;

      auto expression_def =
          unary_expr
        ;

      auto unary_expr_def =
          postfix_expr[helper::assign_action()]
        | ('-' >> -sep >> unary_expr)[
            ([](auto& ctx) {
               _val(ctx) = ast::make_expr<ast::negative_expr>(_attr(ctx));
            })]
        | ('!' >> -sep >> unary_expr)[
            ([](auto& ctx) {
               _val(ctx) = ast::make_expr<ast::bool_negative_expr>(_attr(ctx));
            })]
        ;

      auto postfix_expr_def =
          primary_expr[([](auto& ctx) { _val(ctx) = _attr(ctx); })]
          >> *(
            (real_args)[
              ([](auto& ctx) {
                _val(ctx) = ast::make_expr<ast::apply_expr>(_val(ctx), _attr(ctx));
              })]
          )
        ;

      auto primary_expr_def =
          constant
        | '(' >> -sep >> expression >> -sep >> ')'
        ;

      auto constant_def =
          helper::keyword("true")[
            ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::bool_const_expr>(true); }) ]
        | helper::keyword("false")[
            ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::bool_const_expr>(false); }) ]
        | helper::fp_[
            ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::float_const_expr>(_attr(ctx)); })]
        | x3::int_[
            ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::int_const_expr>(_attr(ctx)); })]
        | lit("()")[
            ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::unit_expr>(); })]
        | identifier[
            ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::ident_expr>(_attr(ctx)); })]
        ;



      auto real_args_def =
          ('(' >> -sep >> ')')[([](auto& ctx) { _val(ctx) = std::vector<ast::expression>{}; })]
        | ('(' >> -sep >> (expression % (',' >> -sep)) >> -sep >> ')')[
            ([](auto& ctx) { _val(ctx) = _attr(ctx); })]
        ;

      BOOST_SPIRIT_DEFINE(
          expression,
          constant,
          identifier,
          primary_expr,
          postfix_expr,
          unary_expr,

          real_args);

    } // namespace grammar
    using grammar::expression;

  } // namespace syntax
} // namespace pebble

#endif
