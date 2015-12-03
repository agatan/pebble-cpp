#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ast
#include <boost/test/unit_test.hpp>
#include <pebble/syntax/ast.hpp>
#include <pebble/utils/stringify.hpp>

BOOST_AUTO_TEST_SUITE(ast)

  BOOST_AUTO_TEST_SUITE(to_string)
    namespace ast = pebble::syntax::ast;

    BOOST_AUTO_TEST_CASE(to_string)
    {
      std::pair<char const*, ast::expression> test_cases[] = {
        {"4", ast::make_expr<ast::int_const_expr>(4)},
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
        std::string expected;
        std::tie(expected, expr) = test_case;
        BOOST_CHECK_EQUAL(expected, pebble::utils::stringify(expr));
      }
    }

  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
