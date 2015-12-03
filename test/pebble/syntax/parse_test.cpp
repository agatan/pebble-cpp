#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE parse

#include <boost/test/unit_test.hpp>

#include <pebble/syntax/ast.hpp>
#include <pebble/syntax/parse.hpp>
#include <pebble/utils/stringify.hpp>

BOOST_AUTO_TEST_SUITE(parse)

  namespace syntax = pebble::syntax;
  namespace ast = pebble::syntax::ast;

  BOOST_AUTO_TEST_CASE(dummy) {
    BOOST_TEST(2 == 1 + 1);
  }

  BOOST_AUTO_TEST_CASE(expression) {
    std::pair<char const*, char const*> test_cases[] = {
      {"TRUE", "true"}
    };

    for (auto&& test_case : test_cases) {
      char const* expected;
      char const* target;
      std::tie(expected, target) = test_case;
      ast::expression expr(*syntax::parse_expression(target));

      BOOST_TEST(expected == pebble::utils::stringify(expr));
    }
  }

BOOST_AUTO_TEST_SUITE_END()
