#ifndef __PEBBLE_SYNTAX_CONFIG_HPP_
#define __PEBBLE_SYNTAX_CONFIG_HPP_

#include <pebble/syntax/skip_grammar.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace pebble {
  namespace syntax {

    // for extract original iterator from X3 context.
    struct orig_begin_iter_tag;

    using string_iterator_t = std::string::const_iterator;
    using iterator_t = boost::spirit::line_pos_iterator<string_iterator_t>;
    using context_type =
      boost::spirit::x3::with_context<
          orig_begin_iter_tag,
          iterator_t,
          boost::spirit::x3::phrase_parse_context<
            decltype(skip)
          >::type
        >::type;

  } // namespace syntax
} // namespace pebble

#endif
