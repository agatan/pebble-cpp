#include <pebble/syntax/parse.hpp>
#include <pebble/syntax/grammar.hpp>
#include <pebble/syntax/config.hpp>

namespace pebble {
  namespace syntax {

    namespace x3 = boost::spirit::x3;

    namespace grammar {
      BOOST_SPIRIT_INSTANTIATE(expression_type, iterator_t, context_type);
      BOOST_SPIRIT_INSTANTIATE(statement_type, iterator_t, context_type);
      BOOST_SPIRIT_INSTANTIATE(definition_type, iterator_t, context_type);
    } // namespace grammar

    grammar::expression_type const& expression()
    {
      return grammar::expression;
    }

    grammar::statement_type const& statement()
    {
      return grammar::statement;
    }

    grammar::definition_type const& definition()
    {
      return grammar::definition;
    }

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
            make_with_context(expression(), begin),
            x3::ascii::space, expr);

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
            make_with_context(statement(), begin),
            x3::ascii::space, stmt);

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
            make_with_context(definition(), begin),
            x3::ascii::space, def);

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
