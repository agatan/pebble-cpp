#ifndef __PEBBLE_SYNTAX_ON_SUCCESS_HPP_
#define __PEBBLE_SYNTAX_ON_SUCCESS_HPP_

#include <pebble/syntax/ast.hpp>
#include <pebble/syntax/config.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/support/iterators/line_pos_iterator.hpp>
#include <iostream>

namespace pebble {
  namespace syntax {
    namespace x3 = boost::spirit::x3;

    // annotate source position for AST node on success.
    struct on_success_base
    {
      template <typename Iterator, typename Attr, typename Context>
      inline void on_success(
          Iterator const& ,
          Iterator const& ,
          Attr& ,
          Context const& ) const
      {
      }

      template <typename Iterator, typename Context>
      inline void on_success(
          Iterator const& first,
          Iterator const& last,
          ast::expression& attr,
          Context const& ctx) const
      {
        on_success_impl(first, last, attr, ctx);
      }

      template <typename Iterator, typename Context>
      inline void on_success(
          Iterator const& first,
          Iterator const& last,
          ast::statement& attr,
          Context const& ctx) const
      {
        on_success_impl(first, last, attr, ctx);
      }

      template <typename Iterator, typename Context>
      inline void on_success(
          Iterator const& first,
          Iterator const& last,
          ast::definition& attr,
          Context const& ctx) const
      {
        on_success_impl(first, last, attr, ctx);
      }

    private:
      template <typename Iterator, typename Attr, typename Context>
      inline void on_success_impl(
          Iterator const& first,
          Iterator const& ,
          Attr& attr,
          Context const& ctx) const
      {
        auto&& orig_begin = x3::get<orig_begin_iter_tag>(ctx);
        auto line = boost::spirit::get_line(first);
        auto col = boost::spirit::get_column(orig_begin, first);
        boost::apply_visitor(
            [line, col](auto&& ast) { ast->line = line; ast->col = col; },
            attr);
      }

    };

  } // namespace syntax
} // namespace pebble

#endif
