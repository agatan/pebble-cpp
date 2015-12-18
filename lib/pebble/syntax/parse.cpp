#include <pebble/syntax/parse.hpp>
#include <pebble/syntax/skip_grammar.hpp>
#include <pebble/syntax/grammar.hpp>

namespace pebble {
  namespace syntax {

    namespace x3 = boost::spirit::x3;

    template <typename Grammar, typename Iter>
    decltype(auto) make_with_context(Grammar&& grammar, Iter&& orig_begin)
    {
      return x3::with<orig_begin_iter_tag>(orig_begin)[
               grammar
             ];
    }

    boost::optional<ast::expression> parse_expression(
        iterator_t& it,
        iterator_t const& begin,
        iterator_t const& end)
    {
      ast::expression expr;
      bool const success =
        boost::spirit::x3::phrase_parse(
            it, end,
            make_with_context(expression, begin),
            skip, expr);

      if (!success || it != end) {
        return boost::none;
      }
      return expr;
    }


    boost::optional<ast::expression> parse_expression(std::string const& src)
    {
      iterator_t it(src.cbegin());
      iterator_t const begin(src.cbegin());
      iterator_t const end(src.cend());

      return parse_expression(it, begin, end);
    }

    boost::optional<ast::statement> parse_statement(
        iterator_t& it,
        iterator_t const& begin,
        iterator_t const& end)
    {
      ast::statement stmt;
      bool const success =
        boost::spirit::x3::phrase_parse(
            it, end,
            make_with_context(statement, begin),
            skip, stmt);

      if (!success || it != end) {
        return boost::none;
      }
      return stmt;
    }


    boost::optional<ast::statement> parse_statement(std::string const& src)
    {
      iterator_t it(src.cbegin());
      iterator_t const begin(src.cbegin());
      iterator_t const end(src.cend());

      return parse_statement(it, begin, end);
    }


    boost::optional<ast::definition> parse_definition(
        iterator_t& it,
        iterator_t const& begin,
        iterator_t const& end)
    {
      ast::definition def;
      bool const success =
        boost::spirit::x3::phrase_parse(
            it, end,
            make_with_context(definition, begin),
            skip, def);

      if (!success || it != end) {
        return boost::none;
      }
      return def;
    }


    boost::optional<ast::definition> parse_definition(std::string const& src)
    {
      iterator_t it(src.cbegin());
      iterator_t const begin(src.cbegin());
      iterator_t const end(src.cend());

      return parse_definition(it, begin, end);
    }

  } // namespace syntax
} // namespace pebble
