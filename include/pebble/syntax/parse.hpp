#ifndef __PEBBLE_SYNTAX_PARSE_HPP_
#define __PEBBLE_SYNTAX_PARSE_HPP_

#include <pebble/syntax/ast.hpp>
#include <pebble/syntax/config.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/optional/optional.hpp>

namespace pebble {
  namespace syntax {

    namespace grammar {
      class expression_class;
      using expression_type =
        boost::spirit::x3::rule<expression_class, ast::expression>;
      BOOST_SPIRIT_DECLARE(expression_type);

      class statement_class;
      using statement_type =
        boost::spirit::x3::rule<statement_class, ast::statement>;
      BOOST_SPIRIT_DECLARE(statement_type);

      class definition_class;
      using definition_type =
        boost::spirit::x3::rule<definition_class, ast::definition>;
      BOOST_SPIRIT_DECLARE(definition_type);
    } // namespace grammar

    grammar::expression_type const& expression();
    grammar::statement_type const& statement();
    grammar::definition_type const& definition();

    template <typename Iter, typename Grammar>
    decltype(auto) with_orig_iter(Iter&& orig_begin, Grammar&& grammar)
    {
      return boost::spirit::x3::with<orig_begin_iter_tag>(orig_begin)[
               grammar
             ];
    }

    template <typename Grammar>
    decltype(auto) with_error_handler(
        boost::spirit::x3::error_handler<iterator_t>& err,
        Grammar&& grammar)
    {
      return boost::spirit::x3::with<boost::spirit::x3::error_handler_tag>(std::ref(err))[
               grammar
             ];
    }


    boost::optional<ast::expression>
      parse_expression(iterator_t&, string_iterator_t const&, iterator_t const&);

    boost::optional<ast::expression>
      parse_expression(std::string const&);

    boost::optional<ast::statement>
      parse_statement(iterator_t&, string_iterator_t const&, iterator_t const&);

    boost::optional<ast::statement>
      parse_statement(std::string const&);

    boost::optional<ast::definition>
      parse_definition(iterator_t&, string_iterator_t const&, iterator_t const&);

    boost::optional<ast::definition>
      parse_definition(std::string const&);

  } // namespace syntax
} // namespace pebble

#endif
