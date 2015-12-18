#ifndef __PEBBLE_SYNTAX_POSITION_HPP_
#define __PEBBLE_SYNTAX_POSITION_HPP_

#include <pebble/syntax/ast.hpp>
#include <boost/variant/apply_visitor.hpp>

namespace pebble {
  namespace syntax {

    template <typename Node>
    std::size_t get_line(Node const& node);

    template <typename Node>
    std::size_t get_col(Node const& node);

    template <typename Node>
    std::size_t get_len(Node const& node);

    namespace detail {

      struct get_line_impl
      {
        template <typename Node>
        std::size_t operator()(Node const& node) const
        {
          return node->line;
        }

        std::size_t operator()(ast::binop_expr_ptr const& binop) const
        {
          return get_line(binop->lhs());
        }

        std::size_t operator()(ast::apply_expr_ptr const& app) const
        {
          return get_line(app->function());
        }
      };

      struct get_col_impl
      {
        template <typename Node>
        std::size_t operator()(Node const& node) const
        {
          return node->col;
        }

        std::size_t operator()(ast::binop_expr_ptr const& binop) const
        {
          return get_col(binop->lhs());
        }

        std::size_t operator()(ast::apply_expr_ptr const& app) const
        {
          return get_col(app->function());
        }
      };

      struct get_len_impl
      {
        template <typename Node>
        std::size_t operator()(Node const& node) const
        {
          return node->len;
        }

        std::size_t operator()(ast::binop_expr_ptr const& binop) const
        {
          auto lhs_col = get_col(binop->lhs());
          return binop->col + binop->len - lhs_col;
        }

        std::size_t operator()(ast::apply_expr_ptr const& app) const
        {
          auto fun_col = get_col(app->function());
          return app->col + app->len - fun_col;
        }
      };

    } // namespace detail

    template <typename Node>
    std::size_t get_line(Node const& node)
    {
      return boost::apply_visitor(detail::get_line_impl{}, node);
    }

    template <typename Node>
    std::size_t get_col(Node const& node)
    {
      return boost::apply_visitor(detail::get_col_impl{}, node);
    }

    template <typename Node>
    std::size_t get_len(Node const& node)
    {
      return boost::apply_visitor(detail::get_len_impl{}, node);
    }

  } // namespace syntax
} // namespace pebble

#endif
