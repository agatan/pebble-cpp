#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE parse
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(parse)

  BOOST_AUTO_TEST_CASE(dummy) {
    BOOST_TEST(2 == 1 + 1);
  }

BOOST_AUTO_TEST_SUITE_END()
