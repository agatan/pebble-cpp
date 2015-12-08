#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE parse

#include <boost/test/unit_test.hpp>

#include <boost/variant/get.hpp>

#include <pebble/syntax/ast.hpp>
#include <pebble/syntax/parse.hpp>
#include <pebble/utils/stringify.hpp>

BOOST_AUTO_TEST_SUITE(parse)

  namespace syntax = pebble::syntax;
  namespace ast = pebble::syntax::ast;

  BOOST_AUTO_TEST_CASE(dummy) {
    BOOST_TEST(2 == 1 + 1);
  }

  BOOST_AUTO_TEST_CASE(constants) {
    ast::expression expr;

    expr = *syntax::parse_expression("true");
    BOOST_TEST(boost::get<ast::bool_const_expr_ptr>(expr)->data() == true);

    expr = *syntax::parse_expression("false");
    BOOST_TEST(boost::get<ast::bool_const_expr_ptr>(expr)->data() == false);

    expr = *syntax::parse_expression("123");
    BOOST_TEST(boost::get<ast::int_const_expr_ptr>(expr)->data() == 123);

    expr = *syntax::parse_expression("123.45");
    BOOST_CHECK_CLOSE(boost::get<ast::float_const_expr_ptr>(expr)->data(), 123.45, 0.0001);

    expr = *syntax::parse_expression("()");
    BOOST_TEST(boost::get<ast::unit_expr_ptr>(&expr));

    expr = *syntax::parse_expression("ident");
    BOOST_TEST(boost::get<ast::ident_expr_ptr>(expr)->name() == "ident");
  }

  BOOST_AUTO_TEST_CASE(apply) {
    ast::expression expr(*syntax::parse_expression("fun(1, 3)"));
    BOOST_REQUIRE(boost::get<ast::apply_expr_ptr>(&expr));
    BOOST_TEST("(APPLY (IDENT fun) 1 3)" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("fun(1, 3)(2)");
    BOOST_REQUIRE(boost::get<ast::apply_expr_ptr>(&expr));
    BOOST_TEST("(APPLY (APPLY (IDENT fun) 1 3) 2)" == pebble::utils::stringify(expr));
  }

  BOOST_AUTO_TEST_CASE(unary) {
    ast::expression expr(*syntax::parse_expression("-a"));
    BOOST_TEST("(NEG (IDENT a))" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("!true");
    BOOST_TEST("(NOT TRUE)" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("-fun()");
    BOOST_TEST("(NEG (APPLY (IDENT fun)))" == pebble::utils::stringify(expr));
  }

  BOOST_AUTO_TEST_CASE(binary) {
    ast::expression expr(*syntax::parse_expression("1 * 2"));
    BOOST_TEST("(* 1 2)" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("1 / 2");
    BOOST_TEST("(/ 1 2)" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("1 + 2");
    BOOST_TEST("(+ 1 2)" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("1 - 2");
    BOOST_TEST("(- 1 2)" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("1 - 2 / 3");
    BOOST_TEST("(- 1 (/ 2 3))" == pebble::utils::stringify(expr));

    expr = *syntax::parse_expression("1 * 2 + 3");
    BOOST_TEST("(+ (* 1 2) 3)" == pebble::utils::stringify(expr));
  }

  BOOST_AUTO_TEST_CASE(expression) {
    std::pair<char const*, char const*> test_cases[] = {
      {"TRUE", "true"},
      {"FALSE", "false"},
      {"123", "123"},
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
