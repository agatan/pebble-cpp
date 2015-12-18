#ifndef __PEBBLE_SYNTAX_PARSE_HPP_
#define __PEBBLE_SYNTAX_PARSE_HPP_

#include <pebble/syntax/ast.hpp>

#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/optional/optional.hpp>

namespace pebble {
  namespace syntax {

    using string_iterator_t = std::string::const_iterator;
    using iterator_t = boost::spirit::line_pos_iterator<string_iterator_t>;

    // for extract error handler from X3 context.
    struct error_handler_tag;

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
