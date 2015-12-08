#include <pebble/syntax/parse.hpp>
#include <pebble/syntax/skip_grammar.hpp>
#include <pebble/syntax/grammar.hpp>

namespace pebble {
  namespace syntax {

    boost::optional<ast::expression> parse_expression(
        iterator_t& it,
        string_iterator_t const& begin,
        iterator_t const& end)
    {
      ast::expression expr;
      bool const success =
        boost::spirit::x3::phrase_parse(it, end, expression, skip, expr);

      if (!success || it != end) {
        return boost::none;
      }
      return expr;
    }


    boost::optional<ast::expression> parse_expression(std::string const& src)
    {
      iterator_t it(src.cbegin());
      string_iterator_t const begin(src.cbegin());
      iterator_t const end(src.cend());

      return parse_expression(it, begin, end);
    }

    boost::optional<ast::statement> parse_statement(
        iterator_t& it,
        string_iterator_t const& begin,
        iterator_t const& end)
    {
      ast::statement stmt;
      bool const success =
        boost::spirit::x3::phrase_parse(it, end, statement, skip, stmt);

      if (!success || it != end) {
        return boost::none;
      }
      return stmt;
    }


    boost::optional<ast::statement> parse_statement(std::string const& src)
    {
      iterator_t it(src.cbegin());
      string_iterator_t const begin(src.cbegin());
      iterator_t const end(src.cend());

      return parse_statement(it, begin, end);
    }
  } // namespace syntax
} // namespace pebble
