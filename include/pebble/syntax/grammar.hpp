#ifndef __PEBBLE_SYNTAX_GRAMMAR_HPP_
#define __PEBBLE_SYNTAX_GRAMMAR_HPP_

#include <pebble/syntax/ast.hpp>
#include <pebble/syntax/on_success.hpp>

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

        // annotate source position to AST
        decltype(auto) annotate_pos()
        {
          return [](auto&& ctx) {
            auto&& orig_begin = x3::get<orig_begin_iter_tag>(ctx);
            auto&& first = _attr(ctx).begin();
            auto&& last = _attr(ctx).end();
            auto line = boost::spirit::get_line(first);
            auto col = boost::spirit::get_column(orig_begin, first);
            auto len = std::distance(first, last);
            node_annotate(_val(ctx), line, col, len);
          };
        }

        template <typename Rule>
        decltype(auto) annotate(Rule&& rule)
        {
          return x3::raw[std::forward<Rule>(rule)][annotate_pos()];
        }

      } // namespace helper

#define PEBBLE_RULE(name,result,base) \
      class name##_class : base {};                   \
      x3::rule<name##_class, result> const name;

      PEBBLE_RULE(expression, ast::expression, public on_success_base);
      PEBBLE_RULE(constant, ast::expression, public on_success_base);
      PEBBLE_RULE(primary_expr, ast::expression, public on_success_base);
      PEBBLE_RULE(postfix_expr, ast::expression, public on_success_base);
      PEBBLE_RULE(unary_expr, ast::expression, public on_success_base);
      PEBBLE_RULE(mul_div_expr, ast::expression, public on_success_base);
      PEBBLE_RULE(add_sub_expr, ast::expression, public on_success_base);
      PEBBLE_RULE(if_expr, ast::expression, public on_success_base);
      PEBBLE_RULE(block_expr, ast::expression, public on_success_base);
      PEBBLE_RULE(non_block_expr, ast::expression, public on_success_base);

      PEBBLE_RULE(statement, ast::statement, public on_success_base);

      PEBBLE_RULE(definition, ast::definition, public on_success_base);
      PEBBLE_RULE(fun_def, ast::definition, public on_success_base);
      PEBBLE_RULE(let_def, ast::definition, public on_success_base);

      x3::rule<class identifier, std::string> const identifier;
      x3::rule<class uidentifier, std::string> const uidentifier;
      x3::rule<class real_args, std::vector<ast::expression>> const real_args;
      x3::rule<class arg, std::pair<std::string, ast::type>> const arg;
      x3::rule<class def_args, std::vector<std::pair<std::string, ast::type>>> const def_args;

      x3::rule<class type, ast::type> const type;


      // base or helper rules
      auto identifier_def = x3::lexeme[
          x3::lower >> *(x3::alnum | char_('_'))
        ]
        ;

      auto uidentifier_def = x3::lexeme[
          x3::upper >> *(x3::alnum | char('_'))
        ]
        ;

      auto real_args_def =
          (lit('(') >> ')')[([](auto& ctx) { _val(ctx) = std::vector<ast::expression>{}; })]
        | ('(' >> (expression % ',') >> ')')[
            ([](auto& ctx) { _val(ctx) = _attr(ctx); })]
        ;

      auto arg_def =
          (
            identifier > ':' > type
          ) [
            ([](auto& ctx) {
              _val(ctx) = std::make_pair(
                  boost::fusion::at_c<0>(_attr(ctx)),
                  boost::fusion::at_c<1>(_attr(ctx))
                );
            })
          ]
        ;

      auto def_args_def =
          (lit('(') >> ')')[([](auto& ctx) { _val(ctx) = std::vector<std::pair<std::string, ast::type>>(); })]
        | ('(' >> (arg % ',') >> ')') [helper::assign_action()]
        ;

      auto type_def =
          uidentifier[
            ([](auto& ctx) { _val(ctx) = ast::type(_attr(ctx)); })
          ]
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
            helper::keyword("if") > '(' > expression > ')'
            > expression > -(helper::keyword("else") > expression)
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
          >> *helper::annotate(
            ('-' >> mul_div_expr)[helper::make_binop_tail("-")]
          | ('+' >> mul_div_expr)[helper::make_binop_tail("+")]
          )
        ;

      auto mul_div_expr_def =
          unary_expr[helper::assign_action()]
          >> *helper::annotate(
            ('*' >> unary_expr)[helper::make_binop_tail("*")]
          | ('/' >> unary_expr)[helper::make_binop_tail("/")]
          )
        ;

      auto unary_expr_def =
          postfix_expr[helper::assign_action()]
        | helper::annotate(
            ('-' >> unary_expr)[
              ([](auto& ctx) {
                 _val(ctx) = ast::make_expr<ast::negative_expr>(_attr(ctx));
              })]
            )
        | helper::annotate(
            ('!' >> unary_expr)[
              ([](auto& ctx) {
                 _val(ctx) = ast::make_expr<ast::bool_negative_expr>(_attr(ctx));
              })]
            )
        ;

      auto postfix_expr_def =
          primary_expr[([](auto& ctx) { _val(ctx) = _attr(ctx); })]
          >> *helper::annotate(
            (real_args)[
              ([](auto& ctx) {
                _val(ctx) = ast::make_expr<ast::apply_expr>(_val(ctx), _attr(ctx));
              })]
          )
        ;

      auto primary_expr_def =
          constant
        | '(' >> expression >> ')'
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



      // definitions
      auto definition_def =
          fun_def[helper::assign_action()]
        | let_def[helper::assign_action()]
        ;

      auto fun_def_def =
          (
            helper::keyword("def") > identifier > def_args > -(':' > type)
            > '=' > expression
          ) [
            ([](auto& ctx) {
               std::string const& name(boost::fusion::at_c<0>(_attr(ctx)));
               std::vector<std::pair<std::string, ast::type>> const&
                 args(boost::fusion::at_c<1>(_attr(ctx)));
               boost::optional<ast::type> const& ret(boost::fusion::at_c<2>(_attr(ctx)));
               ast::expression const& body(boost::fusion::at_c<3>(_attr(ctx)));

               if (ret) {
                 _val(ctx) = ast::make_definition<ast::function_def>(
                     name,
                     args,
                     *ret,
                     body
                 );
               } else {
                 _val(ctx) = ast::make_definition<ast::function_def>(
                     name,
                     args,
                     ast::type("Unit"),
                     body
                 );
               }
            })
          ]
        ;

      auto let_def_def =
          (
            helper::keyword("let") > identifier > -(':' > type)
            > '=' > expression > ';'
          ) [
            ([](auto& ctx) {
              std::string const& name(boost::fusion::at_c<0>(_attr(ctx)));
              auto const& type(boost::fusion::at_c<1>(_attr(ctx)));
              auto const& expr(boost::fusion::at_c<2>(_attr(ctx)));

              if (type) {
                _val(ctx) = ast::make_definition<ast::let_def>(
                    name,
                    *type,
                    expr
                );
              } else {
                _val(ctx) = ast::make_definition<ast::let_def>(
                    name,
                    expr
                );
              }
            })
          ]
        ;


      BOOST_SPIRIT_DEFINE(
          expression,
          constant,
          primary_expr,
          postfix_expr,
          unary_expr,
          mul_div_expr,
          add_sub_expr,
          if_expr,
          block_expr,
          non_block_expr,

          statement,

          definition,
          fun_def,
          let_def,

          real_args,
          identifier,
          uidentifier,
          def_args,
          arg,
          type
          );

    } // namespace grammar

  } // namespace syntax
} // namespace pebble

#endif
