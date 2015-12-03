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
        {"4", std::make_shared<ast::int_const_expr>(4)},
        {"(IDENT variable)", std::make_shared<ast::ident_expr>("variable")},
        {"TRUE", std::make_shared<ast::bool_const_expr>(true)},
        {"UNIT", std::make_shared<ast::unit_expr>()},
        {"(APPLY (IDENT f) 4 TRUE)",
          std::make_shared<ast::apply_expr>(
              std::make_shared<ast::ident_expr>("f"),
              std::vector<ast::expression>{
                std::make_shared<ast::int_const_expr>(4),
                std::make_shared<ast::bool_const_expr>(true)
              }
            )
        },
        {"(NOT TRUE)",
          std::make_shared<ast::bool_negative_expr>(
              std::make_shared<ast::bool_const_expr>(true))
        },
        {"(NEG 1)", std::make_shared<ast::negative_expr>(
            std::make_shared<ast::int_const_expr>(1))},
        {"(IF FALSE 1 2)", std::make_shared<ast::if_expr>(
            std::make_shared<ast::bool_const_expr>(false),
            std::make_shared<ast::int_const_expr>(1),
            std::make_shared<ast::int_const_expr>(2))
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
