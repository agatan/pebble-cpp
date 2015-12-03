#ifndef __PEBBLE_SYNTAX_SKIP_GRAMMER_HPP_
#define __PEBBLE_SYNTAX_SKIP_GRAMMER_HPP_

#include <pebble/syntax/parse.hpp>
#include <pebble/syntax/ast.hpp>

#include <boost/spirit/home/x3.hpp>

namespace pebble {
  namespace syntax {
    namespace x3 = boost::spirit::x3;

    namespace skip_grammar {
      x3::rule<class skip> const skip("skip");

      auto const skip_def =
        (x3::char_(' ') | x3::char_('\t'))
        ;

      BOOST_SPIRIT_DEFINE(skip);
    } // namespace skip_grammar

    using skip_grammar::skip;

  } // namespace syntax
} // namespace pebble

#endif
