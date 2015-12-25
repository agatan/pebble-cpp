#ifndef PEBBLE_SYNTAX_ON_ERROR_HPP_
#define PEBBLE_SYNTAX_ON_ERROR_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

namespace pebble {
  namespace syntax {

    namespace x3 = boost::spirit::x3;


    // reexport error_handler for refactoring.
    template <typename Iter>
    using error_handler = x3::error_handler<Iter>;
    using error_handler_tag = x3::error_handler_tag;

    struct on_error_base
    {
      template<typename Iter, typename Exception, typename Context>
      x3::error_handler_result on_error(
          Iter& , Iter const& ,
          Exception const& ex, Context const& ctx)
      {
        auto message = "Syntax Error. Expecting: " + ex.which() + " here:";
        auto& error_handler = x3::get<error_handler_tag>(ctx).get();
        error_handler(ex.where(), message);
        return x3::error_handler_result::fail;
      }
    };

  } // namespace syntax
} // namespace pebble

#endif
