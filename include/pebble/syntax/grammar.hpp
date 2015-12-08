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

        struct make_binop_tail
        {
          std::string op;

          make_binop_tail(std::string const& o): op(o) {}

          template <typename Context>
          void operator()(Context& ctx)
          {
            _val(ctx) = ast::make_expr<ast::binop_expr>(
                op,
                _val(ctx),
                _attr(ctx)
              );
          }
        };

      } // namespace helper


      x3::rule<class expression, ast::expression> const expression;
      x3::rule<class constant, ast::expression> const constant;
      x3::rule<class primary_expr, ast::expression> const primary_expr;
      x3::rule<class postfix_expr, ast::expression> const postfix_expr;
      x3::rule<class unary_expr, ast::expression> const unary_expr;
      x3::rule<class mul_div_expr, ast::expression> const mul_div_expr;
      x3::rule<class add_sub_expr, ast::expression> const add_sub_expr;
      x3::rule<class if_expr, ast::expression> const if_expr;
      x3::rule<class block_expr, ast::expression> const block_expr;
      x3::rule<class non_block_expr, ast::expression> const non_block_expr;

      x3::rule<class statement, ast::statement> const statement;

      x3::rule<class identifier, std::string> const identifier;
      x3::rule<class real_args, std::vector<ast::expression>> const real_args;


      // base or helper rules
      auto sep = (lit('\n') | "\r\n");

      auto identifier_def = x3::lexeme[
          x3::lower >> *(x3::alnum | char_('_'))
        ]
        ;

      auto real_args_def =
          ('(' >> -sep >> ')')[([](auto& ctx) { _val(ctx) = std::vector<ast::expression>{}; })]
        | ('(' >> -sep >> (expression % (',' >> -sep)) >> -sep >> ')')[
            ([](auto& ctx) { _val(ctx) = _attr(ctx); })]
        ;



      // expressions
      auto expression_def =
          if_expr[helper::assign_action()]
        | block_expr[helper::assign_action()]
        | non_block_expr[helper::assign_action()]
        ;

      auto non_block_expr_def =
          add_sub_expr[helper::assign_action()]
        ;

      auto block_expr_def =
          ('{' >> *statement >> expression >> '}') [
            ([](auto& ctx) {
              _val(ctx) = ast::make_expr<ast::block_expr>(
                  boost::fusion::at_c<0>(_attr(ctx)),
                  boost::fusion::at_c<1>(_attr(ctx))
                );
            })
          ]
        | ('{' >> *statement >> '}') [
            ([](auto& ctx) { _val(ctx) = ast::make_expr<ast::block_expr>(_attr(ctx)); })
          ]
        ;

      auto if_expr_def =
          (
            helper::keyword("if") > -sep > '(' > -sep > expression > -sep > ')'
            > -sep > expression > -sep > -(helper::keyword("else") > -sep > expression)
          )[
            ([](auto& ctx) {
              ast::expression const& cond(boost::fusion::at_c<0>(_attr(ctx)));
              ast::expression const& then_(boost::fusion::at_c<1>(_attr(ctx)));
              auto const& else_(boost::fusion::at_c<2>(_attr(ctx)));
              if (else_) {
                _val(ctx) = ast::make_expr<ast::if_expr>(
                    cond,
                    then_,
                    *else_
                  );
              } else {
                _val(ctx) = ast::make_expr<ast::if_expr>(
                    cond,
                    then_
                  );
              }
            })
          ]
        ;


      auto add_sub_expr_def =
          mul_div_expr[helper::assign_action()]
          >> *(
            ('-' >> -sep >> mul_div_expr)[helper::make_binop_tail("-")]
          | ('+' >> -sep >> mul_div_expr)[helper::make_binop_tail("+")]
          )
        ;

      auto mul_div_expr_def =
          unary_expr[helper::assign_action()]
          >> *(
            ('*' >> -sep >> unary_expr)[helper::make_binop_tail("*")]
          | ('/' >> -sep >> unary_expr)[helper::make_binop_tail("/")]
          )
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



      // statements
      auto statement_def =
          (
            non_block_expr >> ';'
          )[
            ([](auto& ctx) { _val(ctx) = ast::make_stmt<ast::expr_stmt>(_attr(ctx)); })
          ]
        | block_expr[
            ([](auto& ctx) { _val(ctx) = ast::make_stmt<ast::expr_stmt>(_attr(ctx)); })
          ]
        ;


      BOOST_SPIRIT_DEFINE(
          expression,
          constant,
          identifier,
          primary_expr,
          postfix_expr,
          unary_expr,
          mul_div_expr,
          add_sub_expr,
          if_expr,
          block_expr,
          non_block_expr,

          statement,

          real_args);

    } // namespace grammar
    using grammar::expression;
    using grammar::statement;

  } // namespace syntax
} // namespace pebble

#endif
