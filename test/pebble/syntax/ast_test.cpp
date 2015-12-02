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
      std::pair<std::string, ast::expression> test_cases[] = {
        {"4", std::make_shared<ast::int_const>(4)},
        {"(IDENT variable)", std::make_shared<ast::ident>("variable")},
        {"TRUE", std::make_shared<ast::bool_const>(true)},
        {"UNIT", std::make_shared<ast::unit_const>()},
        {"(APPLY (IDENT f) 4 TRUE)",
          std::make_shared<ast::apply>(
              std::make_shared<ast::ident>("f"),
              std::vector<ast::expression>{
                std::make_shared<ast::int_const>(4),
                std::make_shared<ast::bool_const>(true)
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
