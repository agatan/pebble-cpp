#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE parse

#include <boost/test/unit_test.hpp>

#include <boost/variant/get.hpp>

#include <pebble/syntax/ast.hpp>
#include <pebble/syntax/parse.hpp>
#include <pebble/utils/stringify.hpp>
#include <pebble/syntax/position.hpp>
#include <pebble/syntax/on_error.hpp>

#include <sstream>

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

  BOOST_AUTO_TEST_CASE(if_expression) {
    std::pair<char const*, char const*> test_cases[] = {
      {"(IF TRUE (BLOCK_EXPR (STMT 1)) (BLOCK_EXPR (STMT 2)))", "if (true) { 1 } else { 2 }"}
    };

    for (auto&& test_case : test_cases) {
      char const* expected;
      char const* src;
      std::tie(expected, src) = test_case;
      ast::expression expr(*syntax::parse_expression(src));
      BOOST_TEST(expected == pebble::utils::stringify(expr));
    }
  }

  BOOST_AUTO_TEST_CASE(block) {
    std::pair<char const*, char const*> test_cases[] = {
      {"(BLOCK_EXPR (STMT (APPLY (IDENT f))) (STMT TRUE))", "{ f(); true }"},
      {"(BLOCK (STMT (APPLY (IDENT f))) (STMT (APPLY (IDENT g))))",
       "{ f(); g(); }"},
      {"(BLOCK_EXPR (STMT (APPLY (IDENT f))) (STMT (BLOCK_EXPR (STMT 1))))",
       "{ f(); { 1 }}"}
    };

    for (auto&& test_case : test_cases) {
      char const* expected;
      char const* src;
      std::tie(expected, src) = test_case;
      ast::expression expr(*syntax::parse_expression(src));
      BOOST_TEST(expected == pebble::utils::stringify(expr));
    }
  }


  BOOST_AUTO_TEST_CASE(expression_statement) {
    std::pair<char const*, char const*> test_cases[] = {
      {"(STMT (APPLY (IDENT f)))", "f();"}
    };

    for (auto&& test_case : test_cases) {
      char const* expected;
      char const* src;
      std::tie(expected, src) = test_case;
      ast::statement stmt(*syntax::parse_statement(src));
      BOOST_TEST(expected == pebble::utils::stringify(stmt));
    }
  }


  BOOST_AUTO_TEST_CASE(function) {
    std::pair<char const*, char const*> test_cases[] = {
      {"(DEF f ((x (TYPE Int)) (y (TYPE Float))) (RET (TYPE Bool)) (BODY (BLOCK_EXPR (STMT TRUE))))",
       "def f(x: Int, y: Float): Bool = { true }"
      },
      {"(DEF f () (RET (TYPE Unit)) (BODY UNIT))",
       "def f() = ()"
      }
    };

    for (auto&& test_case : test_cases) {
      char const* expected;
      char const* src;
      std::tie(expected, src) = test_case;
      ast::definition def(*syntax::parse_definition(src));
      BOOST_TEST(expected == pebble::utils::stringify(def));
    }
  }


  BOOST_AUTO_TEST_CASE(let) {
    std::pair<char const*, char const*> test_cases[] = {
      {"(LET (x (TYPE Int)) 1)", "let x: Int = 1;"},
      {"(LET y (+ 1 2))", "let y = 1 + 2;"}
    };

    for (auto&& test_case : test_cases) {
      char const* expected;
      char const* src;
      std::tie(expected, src) = test_case;
      ast::definition def(*syntax::parse_definition(src));
      BOOST_TEST(expected == pebble::utils::stringify(def));
    }
  }

  BOOST_AUTO_TEST_CASE(pos) {
    auto src = "1 - 3";
    auto expr = *syntax::parse_expression(src);
    BOOST_TEST(1 == syntax::get_line(expr));
    BOOST_TEST(1 == syntax::get_col(expr));
    BOOST_TEST(5 == syntax::get_len(expr));

    src = "func(1, 2, 3)";
    expr = *syntax::parse_expression(src);
    BOOST_TEST(1 == syntax::get_line(expr));
    BOOST_TEST(1 == syntax::get_col(expr));
    BOOST_TEST(13 == syntax::get_len(expr));
  }

  BOOST_AUTO_TEST_CASE(failed_message) {
    std::string src("func(1, 2");
    std::stringstream err_out;
    syntax::iterator_t it(std::cbegin(src));
    syntax::iterator_t const orig_begin(std::cbegin(src));
    syntax::iterator_t const last(std::cend(src));
    syntax::error_handler<syntax::iterator_t> handler(it, last, err_out, "_none_");

    auto parser =
      syntax::with_orig_iter(orig_begin,
          syntax::with_error_handler(handler,syntax::expression()))
      ;

    ast::expression expr;
    bool const success =
      boost::spirit::x3::phrase_parse(
          it, last,
          parser,
          boost::spirit::x3::ascii::space, expr);

    if (success && it == last) {
      err_out << pebble::utils::stringify(expr);
    }

    char const* expected = "In file _none_, line 1:\n"
                           "Syntax Error. Expecting: ')' here:\n"
                           "func(1, 2\n"
                           "_________^_\n";
    BOOST_TEST(expected == err_out.str());
  }

BOOST_AUTO_TEST_SUITE_END()
