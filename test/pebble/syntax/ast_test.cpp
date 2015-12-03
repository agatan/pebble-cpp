#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ast
#include <boost/test/unit_test.hpp>
#include <pebble/syntax/ast.hpp>
#include <pebble/utils/stringify.hpp>

BOOST_AUTO_TEST_SUITE(ast)

  BOOST_AUTO_TEST_SUITE(to_string)
    namespace ast = pebble::syntax::ast;

    BOOST_AUTO_TEST_CASE(expression_to_string)
    {
      std::pair<char const*, ast::expression> test_cases[] = {
        {"4", ast::make_expr<ast::int_const_expr>(4)},
        {"1.000000", ast::make_expr<ast::float_const_expr>(1.0)},
        {"(IDENT variable)", ast::make_expr<ast::ident_expr>("variable")},
        {"TRUE", ast::make_expr<ast::bool_const_expr>(true)},
        {"UNIT", ast::make_expr<ast::unit_expr>()},
        {"(APPLY (IDENT f) 4 TRUE)",
          ast::make_expr<ast::apply_expr>(
              ast::make_expr<ast::ident_expr>("f"),
              std::vector<ast::expression>{
                ast::make_expr<ast::int_const_expr>(4),
                ast::make_expr<ast::bool_const_expr>(true)
              }
            )
        },
        {"(NOT TRUE)",
          ast::make_expr<ast::bool_negative_expr>(
              ast::make_expr<ast::bool_const_expr>(true))
        },
        {"(NEG 1)", ast::make_expr<ast::negative_expr>(
            ast::make_expr<ast::int_const_expr>(1))},
        {"(IF FALSE 1 2)", ast::make_expr<ast::if_expr>(
            ast::make_expr<ast::bool_const_expr>(false),
            ast::make_expr<ast::int_const_expr>(1),
            ast::make_expr<ast::int_const_expr>(2))
        },
        {"(+ 1 2)", ast::make_expr<ast::binop_expr>("+",
            ast::make_expr<ast::int_const_expr>(1),
            ast::make_expr<ast::int_const_expr>(2))
        },
        {"(BLOCK 1 TRUE UNIT 3)", ast::make_expr<ast::block_expr>(
            std::vector<ast::expression> {
              ast::make_expr<ast::int_const_expr>(1),
              ast::make_expr<ast::bool_const_expr>(true),
              ast::make_expr<ast::unit_expr>(),
              ast::make_expr<ast::int_const_expr>(3)
            }
          )
        }
      };

      for (auto&& test_case : test_cases) {
        ast::expression expr;
        char const* expected;
        std::tie(expected, expr) = test_case;
        BOOST_TEST(expected == pebble::utils::stringify(expr));
      }
    }

    BOOST_AUTO_TEST_CASE(type_to_string) {
      BOOST_TEST("(TYPE Int)" == ast::type("Int").to_string());
    }

    BOOST_AUTO_TEST_CASE(definition_to_string) {
      using arg = std::pair<std::string, ast::type>;

      std::pair<char const*, ast::definition> test_cases[] = {
        {"(DEF f ((x (TYPE Int)) (y (TYPE Float))) (RET (TYPE Bool)) (BODY (BLOCK TRUE)))",
          ast::make_definition<ast::function_def>("f",
              std::vector<arg> {
                std::make_pair("x", ast::type("Int")),
                std::make_pair("y", ast::type("Float"))
              },
              ast::type("Bool"),
              ast::make_expr<ast::block_expr>(
                std::vector<ast::expression>{ast::make_expr<ast::bool_const_expr>(true)}
              )
            )
        },
        {"(LET (x (TYPE Int)) 1)",
          ast::make_definition<ast::let_def>("x", ast::type("Int"),
              ast::make_expr<ast::int_const_expr>(1))
        },
      };

      for (auto&& test_case : test_cases) {
        char const* expected;
        ast::definition def;
        std::tie(expected, def) = test_case;
        BOOST_TEST(expected == pebble::utils::stringify(def));
      }
    }

  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
